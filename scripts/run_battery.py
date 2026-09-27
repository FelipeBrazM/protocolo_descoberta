"""Run inside opp_env; launch and clean up our own Veins/SUMO daemon."""
import os, pathlib, subprocess, sys, time, socket
root=pathlib.Path(__file__).resolve().parents[1]
os.chdir(root)
veins=pathlib.Path(os.environ.get('VEINS_ROOT',root.parent/'veins-5.3.1'))
os.environ['LD_LIBRARY_PATH']=str(veins/'src')+':'+os.environ.get('LD_LIBRARY_PATH','')
s=socket.socket(); s.bind(('127.0.0.1',0)); port=s.getsockname()[1]; s.close()
(root/'results/battery').mkdir(parents=True,exist_ok=True)
with open(root/'results/battery/launchd.log','w') as log:
    daemon=subprocess.Popen([str(veins/'bin/veins_launchd'),'-p',str(port),'-c',str(root.parent/'sumo-1_22_0/bin/sumo')],stdout=log,stderr=log)
    try:
        for _ in range(100):
            if daemon.poll() is not None: raise RuntimeError('launchd failed: see results/battery/launchd.log')
            try:
                with socket.create_connection(('127.0.0.1',port),timeout=.1): break
            except OSError: time.sleep(.05)
        subprocess.run(['./protocolo_descoberta','-u','Cmdenv','-n',f'.:{veins}/src/veins','-f','battery.ini','-c',sys.argv[1] if len(sys.argv)>1 else 'BatteryUrban',f'--*.manager.port={port}']+sys.argv[2:],check=True)
    finally:
        daemon.terminate()
        try: daemon.wait(timeout=5)
        except subprocess.TimeoutExpired: daemon.kill(); daemon.wait()
