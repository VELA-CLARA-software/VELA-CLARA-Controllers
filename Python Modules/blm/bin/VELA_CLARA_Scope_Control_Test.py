import sys,os
import time
# import randomPVs
#os.environ["EPICS_CA_AUTO_ADDR_LIST"] = "NO"
#os.environ["EPICS_CA_ADDR_LIST"] = "10.10.0.12"
#os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "10000000"

import VELA_CLARA_Scope_Control as vcsc
import epics

def main():
	s=vcsc.init()
	p=s.virtual_VELA_INJ_Scope_Controller()
	numShots = 1
	sc01 = 'WVF01'
	tr1 = vcsc.SCOPE_PV_TYPE.TR1
	tr2 = vcsc.SCOPE_PV_TYPE.TR2
	tr3 = vcsc.SCOPE_PV_TYPE.TR3
	tr4 = vcsc.SCOPE_PV_TYPE.TR4
	p.monitorATraceForShots(numShots)
	while p.isMonitoringScopeTrace(sc01,tr1):
		time.sleep(0.01)
	q = p.getScopeTraceDataStruct(sc01)
	print p.getScopeNames()
	print 'SCOPE 01 P Data'
	#print 'name = ', q.name
	#for i in range(0,numShots):
	#	print 'p1 = ', q.numData[tr1][i], 'timestamp = ', q.numTimeStamps[tr1][i]
	#	print 'p2 = ', q.numData[tr2][i], 'timestamp = ', q.numTimeStamps[tr2][i]
	#	print 'p3 = ', q.numData[tr3][i], 'timestamp = ', q.numTimeStamps[tr3][i]
	#	print 'p4 = ', q.numData[tr4][i], 'timestamp = ', q.numTimeStamps[tr4][i]
		
if __name__ == "__main__":
	main()
