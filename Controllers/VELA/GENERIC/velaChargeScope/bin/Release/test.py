import velaChargeScopeControl as VCSC
import time
import sys, os
import numpy
os.environ["EPICS_CA_AUTO_ADDR_LIST"] = "NO"
os.environ["EPICS_CA_ADDR_LIST"] = "192.168.83.255"
os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "10000000000"

s = VCSC.velaChargeScopeController()

trace1 = VCSC.SCOPE_PV_TYPE.TR1
trace2 = VCSC.SCOPE_PV_TYPE.TR2
trace3 = VCSC.SCOPE_PV_TYPE.TR3
trace4 = VCSC.SCOPE_PV_TYPE.TR4
while True:
	start = time.clock()

	s.monitorTracesForNShots(10)

	while s.isMonitoringScopeTraces() == True:
		time.sleep(0.1)

	ict1 = s.getAreaUnderTraces("WVF01",trace1)
	ict2 = s.getAreaUnderTraces("WVF01",trace2)
	ict3 = s.getAreaUnderTraces("WVF01",trace3)
	ict4 = s.getAreaUnderTraces("WVF01",trace4)
	wcm = s.getWCMQ()
	print time.clock() - start
	time.sleep(1)
	print numpy.mean(ict1)
	print numpy.mean(ict2)
	print numpy.mean(ict3)
	print numpy.mean(ict4)
	while s.isMonitoringScopeTraces() == True:
		time.sleep(0.1)

print "whaaaaa"
#timestamps = s.getTimeStamps("WVF01",trace2)
print numpy.mean(ict1), "       ", wcm

for i in range(0,99):
	print ict1[i]
# 	print timestamps[i]
