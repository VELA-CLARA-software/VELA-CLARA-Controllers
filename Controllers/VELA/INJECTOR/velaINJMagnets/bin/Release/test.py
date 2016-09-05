import sys,os
import numpy as np
#os.environ["EPICS_CA_AUTO_ADDR_LIST"] = "NO"
#os.environ["EPICS_CA_ADDR_LIST"] = "192.168.82.10"
#os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "10000000000"

import velaINJMagnetControl as VIMC

a = VIMC.velaINJMagnetController(True,False)


print( np.array(a.getQuadNames()))
