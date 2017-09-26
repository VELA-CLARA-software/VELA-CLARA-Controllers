import sys,os
import time
import random

#sys.path.append('D:\\VELA-CLARA-Controllers-master\\VELA-CLARA-Controllers\\Controllers\\VELA\\INJECTOR\\velaINJBeamPositionMonitors\\bin\\Release')

import VELA_CLARA_BPM_Control as vbpmc
import epics

#srPV = randomPVs.setRandomPV()
s=vbpmc.init()
p=s.virtual_VELA_INJ_BPM_Controller()
numShots = 1
while True:
	p.setY('BPM01',random.uniform(0.0,2.0))
	p.setX('BPM01',random.uniform(0.0,2.0))
	# tr=p.getYFromPV('BPM01')
	# print tr
	# print p.getBPMXPVBuffer('BPM01')
	print p.getBPMYPVBuffer('BPM01')[-1]
	print p.getYFromPV('BPM01')
	#pv1 = 'VM-EBT-INJ-DIA-BPMC-02:DATA:B2V.VALA'
	#srPV.setPV(pv1,1,10,10,1, "array")
	p.monitorDataForNShots(numShots,'BPM01')
	time.sleep(0.1)
