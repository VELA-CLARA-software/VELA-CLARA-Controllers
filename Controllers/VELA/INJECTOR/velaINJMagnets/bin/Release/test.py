import sys,os

os.environ["EPICS_CA_ADDR_LIST"] = "192.168.82.10"
os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "100000000"


import velaINJMagnetControl as VIMC

a = VIMC.velaINJMagnetController(True,False)
a.switchONpsu('SOL')
#print(a.isON('HVCOR'))
print(a.getRI('SOL'))
#print(a.getILockStates('HVxhfjsfhk01'))
a.switchONpsu('SOL')
#print(a.isON('HVCOR'))
print(a.isON('SOL'))
