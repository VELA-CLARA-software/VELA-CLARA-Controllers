import sys,os
os.environ["EPICS_CA_AUTO_ADDR_LIST"] = "NO"
os.environ["EPICS_CA_ADDR_LIST"] = "192.168.82.10"
os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "10000000000"

import velaRFGunControl as VRFGC

p=VRFGC.velaRFGunController(True,True)
p.setAmp(1222)
