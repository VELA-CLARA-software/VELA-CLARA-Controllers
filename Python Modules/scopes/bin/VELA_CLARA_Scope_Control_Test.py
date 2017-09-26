import sys,os
import time
# import randomPVs
os.environ["EPICS_CA_AUTO_ADDR_LIST"] = "NO"
os.environ["EPICS_CA_ADDR_LIST"] = "10.10.0.12"
os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "10000000"

import VELA_CLARA_Scope_Control as vcsc
import epics

def main():
	s=vcsc.init()
	p=s.virtual_VELA_INJ_Scope_Controller()
	numShots = 1
	sc01 = 'SCOP01'
	tr1 = vcsc.SCOPE_PV_TYPE.P1
	tr2 = vcsc.SCOPE_PV_TYPE.P2
	tr3 = vcsc.SCOPE_PV_TYPE.P3
	tr4 = vcsc.SCOPE_PV_TYPE.P4
	# p.monitorNumsForNShots(numShots)
	# while p.isMonitoringScopeNum(sc01,tr1) or p.isMonitoringScopeNum(sc01,tr2) or p.isMonitoringScopeNum(sc01,tr3) or p.isMonitoringScopeNum(sc01,tr4):
	# 	time.sleep(0.01)
	while True:
		q = p.getScopeNumDataStruct(sc01)
		print p.getScopeNames()
		print 'SCOPE 01 P Data'
		#print 'name = ', q.name
		# for i in range(0,numShots):
		print 'p1 = ', q.numData[tr1][0], 'timestamp = ', q.numTimeStamps[tr1][0]
		print 'p2 = ', q.numData[tr2][0], 'timestamp = ', q.numTimeStamps[tr2][0]
		print 'p3 = ', q.numData[tr3][0], 'timestamp = ', q.numTimeStamps[tr3][0]
		print 'p4 = ', q.numData[tr4][0], 'timestamp = ', q.numTimeStamps[tr4][0]
	time.sleep(0.1)
		
if __name__ == "__main__":
	main()
