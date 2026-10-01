"""Validate fixed-destination service, charging time, and the preserved V2V layer."""
import hashlib, json, math, pathlib, re, shlex, subprocess, sys
ROOT=pathlib.Path(__file__).resolve().parents[1]
OUT=ROOT/'results/battery'
CASES=['BatteryMinimal','BatteryConfirmation','BatteryResponseTimeout','BatteryInsufficient',
       'BatteryRateLimited','BatteryMultihop','BatteryHopLimit','BatteryExpiredFrames','BatteryUrban']
ORIGINAL_ROUTES_SHA256='708e49803f9f2058fc02d8eb148bab44b3e1b0626165d2aa9467e46dcb8900b7'
def read(path):
    scalars={};stats={};current=None
    for line in path.read_text().splitlines():
        a=shlex.split(line)
        if not a:continue
        if a[0]=='scalar':scalars[(a[1],a[2])]=float(a[3]);current=None
        elif a[0]=='statistic':current=(a[1],a[2]);stats[current]={}
        elif a[0]=='field' and current:stats[current][a[1]]=float(a[2])
    return scalars,stats

def events(path):
    result=[]
    for line in path.read_text().splitlines():
        match=re.search(r'\b([A-Z][A-Z_]+) node=',line)
        if match:
            row=dict(re.findall(r'(\w+)=([^\s]+)',line))
            row['event']=match.group(1)
            result.append(row)
    return result

def check_minimal_mobility(log):
    # Veins records mobility under manager; identify the controlled car by its
    # unique initial X, rather than relying on dynamically assigned vector IDs.
    ev=events(log)
    stop=next(e for e in ev if e['event']=='CAR_STOPPED')
    resume=next(e for e in ev if e['event']=='CAR_RESUMED')
    begin=float(stop['simTime']);end=float(resume['simTime'])
    x=float(stop['position'].strip('()').split(',')[0])
    names={};samples={}
    for line in log.with_suffix('.vec').read_text().splitlines():
        a=line.split()
        if a and a[0]=='vector':names[a[1]]=a[3]
        elif len(a)==4 and a[0].isdigit():samples.setdefault(a[0],[]).append((float(a[2]),float(a[3])))
    positions=[v for k,v in samples.items() if names[k]=='posx' and math.isclose(v[0][1],x)]
    assert len(positions)==1
    position=positions[0]
    assert all(math.isclose(v,x,abs_tol=1e-8) for t,v in position if begin<=t<=end)
    assert any(v>x for t,v in position if end<t<end+3)
    speeds=[v for k,v in samples.items() if names[k]=='speed' and max(z for _,z in v)<=.200001]
    assert len(speeds)==1
    assert all(v==0 for t,v in speeds[0] if begin<=t<=end)
    assert any(v>0 for t,v in speeds[0] if end<t<end+3)

def check(case, scalars, stats, log):
    apps=sorted({m for m,k in scalars if m.endswith('.appl') and k=='finalEnergy'})
    trucks=[m for m in apps if '.truck[' in m];cars=[m for m in apps if '.node[' in m]
    assert len(trucks)==(2 if case in ['BatteryUrban','BatteryConfirmation'] else 1),(case,trucks)
    def value(module,key):return scalars.get((module,key),0)
    def total(key,modules=apps):return sum(value(m,key) for m in modules)
    for m in apps:
        expected=stats[(m,'initialEnergy')]['mean']+value(m,'energyReceived')-value(m,'energySupplied')-value(m,'energyConsumed')
        assert math.isclose(expected,value(m,'finalEnergy'),abs_tol=1e-7),(case,m,'energy balance')
        assert value(m,'finalEnergy')>=0
    for m in cars:
        assert value(m,'requests')==sum(value(m,k) for k in ['requestsServed','requestsExpired','requestsPendingAtFinish'])
    for m in trucks:
        assert value(m,'requestsReceived')==value(m,'responsesSent')+value(m,'requestsRejected')
        assert value(m,'responsesSent')==sum(value(m,k) for k in ['requestsAccepted','responseTimeouts','responsesPendingAtFinish'])
        assert value(m,'requestsAccepted')==sum(value(m,k) for k in ['servicesCompleted','servicesExpired','servicesPendingAtFinish'])
    for key in ['messagesTransmitted','messagesReceived','retransmissions','duplicatesDiscarded','messagesExpired','hopLimitDrops']:
        assert math.isclose(total(key),scalars[('Grid9x9Scenario.stats','network.'+key)]),(case,key)
    ev=events(log)
    waiting={};confirmed=set();going={};started={};completed=set();chosen={}
    snapshots={}
    stopped=set(); ended=set()
    for e in ev:
        key=(e['originAddress'],e['requestId'],e['truckId'])
        request=key[:2];name=e['event'];t=float(e['simTime'])
        if name=='REQUEST_SENT':
            if request in snapshots:assert snapshots[request]==e['destination'],('request destination changed',key)
            snapshots[request]=e['destination']
        if name=='CAR_STOPPED':
            assert request in snapshots and request not in stopped
            stopped.add(request)
        if name in ['REQUEST_EXPIRED','ENERGY_TRANSFER_COMPLETED'] and e['node'].startswith('node['):
            assert request in stopped
            ended.add(request)
        if name=='CAR_RESUMED':
            assert request in ended and request in stopped
            stopped.remove(request)
        if name=='MEETING_REACHED':
            assert request in stopped
        if name=='WAITING_CONFIRMATION':waiting[key]=t
        if name=='REQUEST_CONFIRMED':
            assert request not in chosen or chosen[request]==e['truckId']
            chosen[request]=e['truckId']
        if name=='TRUCK_RESPONSE_TIMEOUT':assert math.isclose(t-waiting[key],5,abs_tol=1e-7)
        if name=='TRUCK_CONFIRMATION_RECEIVED':confirmed.add(key)
        if name=='TRUCK_DESTINATION_SET':
            assert key in confirmed
            assert snapshots[request]==e['destination']
            going[key]=e['destination']
        if name=='TRUCK_MEETING':
            assert going[key]==e['destination']
            point=lambda text:tuple(map(float,text.strip('()').split(',')))
            assert math.dist(point(e['position']),point(e['destination']))<=20.02
        if name=='ENERGY_TRANSFER_STARTED' and e['node'].startswith('truck['):
            assert key in going
            assert not any(k[2]==key[2] and k not in completed for k in started),('concurrent service',key)
            started[key]=(t,float(e['energyRequired']))
        if name=='ENERGY_TRANSFER_COMPLETED' and e['node'].startswith('truck['):
            begin,energy=started[key]
            assert key not in completed
            assert math.isclose(t-begin,10,abs_tol=.03),(key,'charging duration')
            completed.add(key)
    supplied=total('energySupplied');received=total('energyReceived')
    assert received<=supplied+1e-7
    if case in ['BatteryMinimal','BatteryConfirmation','BatteryRateLimited','BatteryUrban']:
        assert total('requestsServed',cars)>=1,(case,'no completed car service')
        names={e['event'] for e in ev}
        assert {'REQUEST_SENT','CAR_STOPPED','CAR_RESUMED','TRUCK_RESPONSE_RECEIVED','CONFIRMATION_SENT','REQUEST_RECEIVED','TRUCK_RESPONSE_SENT','REQUEST_CONFIRMED',
                'TRUCK_DESTINATION_SET','TRUCK_GOING_TO_REQUEST','TRUCK_MEETING',
                'ENERGY_TRANSFER_STARTED','ENERGY_TRANSFER_COMPLETED','TRUCK_AVAILABLE'}<=names
    if case=='BatteryMinimal':
        check_minimal_mobility(log)
        assert received==supplied==20 and total('finalEnergy',cars)==40 and total('finalEnergy',trucks)==480
    if case=='BatteryConfirmation':
        assert total('requestsAccepted',trucks)==1 and total('responseTimeouts',trucks)>=1
        assert received==supplied==20
    if case=='BatteryResponseTimeout':
        assert total('responseTimeouts',trucks)>=1 and not going and supplied==0
    if case=='BatteryInsufficient':assert total('requestsRejectedEnergy',trucks)>0 and supplied==0
    if case=='BatteryRateLimited':assert received==40 and supplied==40
    maxhop=max((v.get('max',0) for (m,k),v in stats.items() if m in trucks and k=='requestHopCount'),default=0)
    if case=='BatteryMultihop':assert maxhop>=3 and total('duplicatesDiscarded')>0
    if case in ['BatteryHopLimit','BatteryExpiredFrames']:assert total('requestsReceived',trucks)==0
    if case=='BatteryExpiredFrames':assert total('messagesExpired')>0
    if case=='BatteryUrban':
        assert len(cars)==50
        initial=[stats[(m,'initialEnergy')]['mean'] for m in cars]
        assert all(16<=x<=40 for x in initial) and len(set(initial))==50
        quantities={float(e['energyRequired']) for e in ev if e['event']=='REQUEST_SENT'}
        assert len(quantities)>1
    return dict(case=case,cars=len(cars),trucks=len(trucks),requests=total('requests',cars),
                served=total('requestsServed',cars),supplied=supplied,received=received,
                maxRequestHops=maxhop,responseTimeouts=total('responseTimeouts',trucks),
                completedCharges=len(completed),tx=total('messagesTransmitted'))

if __name__=='__main__':
    assert hashlib.sha256((ROOT/'sumo/grid9x9_50.rou.xml').read_bytes()).hexdigest()==ORIGINAL_ROUTES_SHA256
    OUT.mkdir(parents=True,exist_ok=True);results=[]
    seeds=[int(x) for x in sys.argv[1:]] or [0]
    for seed in seeds:
        for case in CASES:
            stem=OUT/f'{case}-seed{seed}'
            with stem.with_suffix('.log').open('w') as log:
                subprocess.run([sys.executable,str(ROOT/'scripts/run_battery.py'),case,
                                f'--seed-set={seed}',f'--*.manager.seed={seed}',
                                f'--output-scalar-file={stem}.sca',f'--output-vector-file={stem}.vec'],
                               cwd=ROOT,stdout=log,stderr=log,check=True)
            row=check(case,*read(stem.with_suffix('.sca')),stem.with_suffix('.log'))
            row['seed']=seed;results.append(row);print(json.dumps(row),flush=True)
    (OUT/'test-summary.json').write_text(json.dumps(results,indent=2)+'\n')
    print(f'PASS: {len(results)} integrated runs; fixed destinations, confirmation, configurable duration and V2V checked.')
