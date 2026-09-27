"""Build fixed mobility fixtures from actual SUMO connections; never used by apps."""
from pathlib import Path
import xml.etree.ElementTree as ET
from collections import defaultdict, deque
ROOT = Path(__file__).resolve().parents[1]
S = ROOT / 'sumo'
net = ET.parse(S / 'grid9x9.net.xml').getroot()
edges = {e.get('id'): e for e in net.findall('edge') if e.get('function') != 'internal'}
adj = defaultdict(set)
for c in net.findall('connection'):
    if c.get('from') in edges and c.get('to') in edges:
        adj[c.get('from')].add(c.get('to'))
def path(start, end):
    q = deque([[start]]); seen = {start}
    while q:
        p = q.popleft()
        if p[-1] == end: return p
        for nxt in sorted(adj[p[-1]]):
            if nxt not in seen: seen.add(nxt); q.append(p + [nxt])
    raise ValueError((start, end))
route = ['H0_0']
for target in ['H2_7', 'H4_0', 'H6_7', 'H8_0', 'H0_0']:
    route += path(route[-1], target)[1:]
route = route[:-1] * 8 + ['H0_0']
assert all(b in adj[a] and edges[a].get('to') == edges[b].get('from') for a,b in zip(route,route[1:]))
def write(name, root):
    ET.indent(root); ET.ElementTree(root).write(S/name, encoding='utf-8', xml_declaration=True)
r = ET.Element('routes')
ET.SubElement(r,'vType',id='batteryTruck',vClass='truck',length='10',maxSpeed='10',color='0,0.6,1')
v=ET.SubElement(r,'vehicle',id='batteryTruck0',type='batteryTruck',depart='0',departPos='50',departSpeed='5')
ET.SubElement(v,'route',edges=' '.join(route)); write('battery_truck.rou.xml',r)
for name, cars in [('minimal',1),('multiple',6)]:
    r=ET.Element('routes')
    ET.SubElement(r,'vType',id='electricCar',maxSpeed='0.2',sigma='0',color='0,1,0')
    for i in range(cars):
        v=ET.SubElement(r,'vehicle',id=f'ev{i}',type='electricCar',depart=str(i*2),departPos='150',departSpeed='0.2')
        ET.SubElement(v,'route',edges=' '.join(route))
    write(f'battery_{name}.rou.xml',r)
# A controlled line of relays: 60 m spacing, 180 m source-truck separation.
r=ET.Element('routes')
ET.SubElement(r,'vType',id='batteryTruck',vClass='truck',length='10',maxSpeed='1',sigma='0')
v=ET.SubElement(r,'vehicle',id='batteryTruck0',type='batteryTruck',depart='0',departPos='200',departSpeed='1')
ET.SubElement(v,'route',edges=' '.join(route));write('battery_truck_multihop.rou.xml',r)
r=ET.Element('routes');ET.SubElement(r,'vType',id='electricCar',maxSpeed='1',sigma='0')
for i,pos in enumerate([20,80,140]):
    v=ET.SubElement(r,'vehicle',id=f'ev{i}',type='electricCar',depart='0',departPos=str(pos),departSpeed='1')
    ET.SubElement(v,'route',edges=' '.join(route))
write('battery_multihop.rou.xml',r)
r=ET.Element('routes')
ET.SubElement(r,'vType',id='batteryTruck',vClass='truck',length='10',maxSpeed='5',sigma='0')
v=ET.SubElement(r,'vehicle',id='batteryTruck0',type='batteryTruck',depart='0',departPos='150',departSpeed='5')
ET.SubElement(v,'route',edges=' '.join(route));write('battery_truck_approach.rou.xml',r)
# Independent truck patrols, without editing any of the original 50 car routes.
route2 = ['H8_0']
for target in ['H6_0', 'H4_7', 'H2_0', 'H0_7', 'H8_0']:
    route2 += path(route2[-1], target)[1:]
route2 = route2[:-1] * 32 + ['H8_0']
assert all(b in adj[a] for a,b in zip(route2,route2[1:]))
r=ET.Element('routes')
ET.SubElement(r,'vType',id='batteryTruck',vClass='truck',length='10',maxSpeed='10',color='0,0.6,1')
for index, patrol in enumerate([route,route2]):
    v=ET.SubElement(r,'vehicle',id=f'batteryTruck{index}',type='batteryTruck',depart='0',departPos='30',departSpeed='5')
    ET.SubElement(v,'route',edges=' '.join(patrol))
write('battery_two_trucks.rou.xml',r)
competition2 = ['H0_0']
for target in ['H4_7','H8_0','H0_0']:
    competition2 += path(competition2[-1], target)[1:]
competition2 = competition2[:-1]*32 + ['H0_0']
assert all(b in adj[a] for a,b in zip(competition2,competition2[1:]))
r=ET.Element('routes')
ET.SubElement(r,'vType',id='batteryTruck',vClass='truck',length='10',maxSpeed='10',sigma='0')
for i,patrol in enumerate([route,competition2]):
    v=ET.SubElement(r,'vehicle',id=f'batteryTruck{i}',type='batteryTruck',depart='0',departPos=str(50-i*30),departSpeed='0')
    ET.SubElement(v,'route',edges=' '.join(patrol))
write('battery_confirmation_trucks.rou.xml',r)
for name, carfile in [('confirmation','battery_minimal.rou.xml'),('approach','battery_minimal.rou.xml'),('multihop','battery_multihop.rou.xml'),('minimal','battery_minimal.rou.xml'),('multiple','battery_multiple.rou.xml'),('urban','grid9x9_50.rou.xml')]:
    truckfile={'multihop':'battery_truck_multihop.rou.xml','approach':'battery_truck_approach.rou.xml','urban':'battery_two_trucks.rou.xml','confirmation':'battery_confirmation_trucks.rou.xml'}.get(name,'battery_truck.rou.xml')
    c=ET.Element('configuration'); inp=ET.SubElement(c,'input')
    ET.SubElement(inp,'net-file',value='grid9x9.net.xml')
    ET.SubElement(inp,'route-files',value=f'{truckfile},{carfile}')
    t=ET.SubElement(c,'time'); ET.SubElement(t,'end',value='7200')
    processing=ET.SubElement(c,'processing');ET.SubElement(processing,'time-to-teleport',value='-1')
    write(f'battery_{name}.sumocfg',c)
    l=ET.Element('launch'); ET.SubElement(l,'basedir',path='sumo')
    for f in ['grid9x9.net.xml',truckfile,carfile]: ET.SubElement(l,'copy',file=f)
    ET.SubElement(l,'copy',file=f'battery_{name}.sumocfg',type='config')
    write(f'battery_{name}.launchd.xml',l)
print(f'Validated {len(route)} truck edges against explicit connections; generated six scenarios.')
