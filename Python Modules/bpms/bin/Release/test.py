import sys,os
import time
#import randomPVs
os.environ["EPICS_CA_AUTO_ADDR_LIST"] = "NO"
os.environ["EPICS_CA_ADDR_LIST"] = "10.10.0.12"
os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "10000000"

#sys.path.append('D:\\VELA-CLARA-Controllers-master\\VELA-CLARA-Controllers\\Controllers\\VELA\\INJECTOR\\velaINJBeamPositionMonitors\\bin\\Release')

import VELA_CLARA_BPM_Control as vbpmc
import epics

#srPV = randomPVs.setRandomPV()
s=vbpmc.init()
p=s.virtual_VELA_INJ_BPM_Controller()
numShots = 1
time.sleep(1)
tr=p.getYFromPV('BPM01')
print tr
#pv1 = 'VM-EBT-INJ-DIA-BPMC-02:DATA:B2V.VALA'
#srPV.setPV(pv1,1,10,10,1, "array")
#p.monitorDataForNShots(numShots,'BPM01')
#while p.isMonitoringBPMData('BPM01'):
#	time.sleep(0.01)
#q = p.getAllBPMData('BPM01')
#print 'BPM 01 Data'
#print 'name = ', q.name
#for i in range(0,numShots):
#	print 'x = ', q.x[i]
#	print 'y   = ', q.y[i]
#	print 'q   = ', q.q[i]
#	print 'pu1   = ', q.pu1[i]
