import velaINJMagnetControl as vmc
import time
import os
os.environ["EPICS_CA_AUTO_ADDR_LIST"] = "NO"
os.environ["EPICS_CA_ADDR_LIST"] = "192.168.56.101"
os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "100000000"
p = vmc.velaINJMagnetController(True,False)

#p.switchONpsu('SOL')
print(p.isON ('QUAD-01'))
RIcurrent = p.getRI('QUAD-01')
print(RIcurrent)
p.switchONpsu('QUAD-01')
print(p.isON ('QUAD-01'))
#p.switchONpsu('SOL')

#print(p.isON ('SOL'))