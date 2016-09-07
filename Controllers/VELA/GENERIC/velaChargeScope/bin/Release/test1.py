import velaChargeScopeControl as VCSC
import time
import sys, os
import numpy
os.environ["EPICS_CA_AUTO_ADDR_LIST"] = "NO"
os.environ["EPICS_CA_ADDR_LIST"] = "192.168.83.255"
os.environ["EPICS_CA_MAX_ARRAY_BYTES"] = "10000000000"

s = VCSC.velaChargeScopeController()

p1 = VCSC.SCOPE_PV_TYPE.P1
p2 = VCSC.SCOPE_PV_TYPE.P2
p3 = VCSC.SCOPE_PV_TYPE.P3
p4 = VCSC.SCOPE_PV_TYPE.P4
tr1 = VCSC.SCOPE_PV_TYPE.TR1
tr2 = VCSC.SCOPE_PV_TYPE.TR2
tr3 = VCSC.SCOPE_PV_TYPE.TR3
tr4 = VCSC.SCOPE_PV_TYPE.TR4
while True:
	start = time.clock()
	# s.monitorNumsForNShots(10)
	# time.sleep(2)
	while s.isMonitoringScopeNums() == True:
		time.sleep(0.1)

	ict1 = s.getScopeTraces("WVF01", tr1)

	# wcm = s.getWCMQ()
	print time.clock() - start
	time.sleep(1)
	print numpy.mean(ict1[0])
	# for i in range(0,len(ict1[0])):
		# print ict1[0][i]
# 	print timestamps[i]

print "whaaaaa"
#timestamps = s.getTimeStamps("WVF01",trace2)
print numpy.mean(ict1), "       ", wcm

for i in range(0,99):
	print ict1[i]
# 	print timestamps[i]
