import sys,os
import time
os.environ["EPICS_CA_AUTO_ADDR_LIST"] = "NO"
os.environ["EPICS_CA_ADDR_LIST"] = "10.10.0.12"
os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "10000000"

#sys.path.append('D:\\VELA-CLARA-Controllers-master\\VELA-CLARA-Controllers\\Controllers\\VELA\\INJECTOR\\velaINJBeamPositionMonitors\\bin\\Release')

import VELA_CLARA_BPM_Control as vbpmc
import epics

def main():
	s=vbpmc.init()
	p=s.virtual_VELA_INJ_BPM_Controller()
	numShots = 10
	bpmlist = vbpmc.std_vector_string()
	bpmlist.append('BPM02')
	bpmlist.append('BPM01')
	print bpmlist
	p.monitorDataForNShots(numShots,'BPM02')
	for i in bpmlist:
		while p.isMonitoringBPMData(i):
			time.sleep(0.01)
	bpm2x = p.getBPMXVec('BPM02')
	for i in range(numShots)
		print bpm2x[i]
	while True:
		q = p.getAllBPMData('BPM02')
		print len(q.x)
		print 'BPM 02 Data'
		print 'name = ', q.name
		print 'x         = ', q.x[0]
		print 'y         = ', q.y[0]
		print 'q         = ', q.q[0]
		print 'pu1       = ', q.pu1[0]
		print 'timestamp = ', q.timeStamps[0]
		time.sleep(0.1)
		
if __name__ == "__main__":
	main()
