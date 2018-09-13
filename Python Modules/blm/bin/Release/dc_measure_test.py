import epics, time, math, numpy, sys
sys.path.append('\\\fed.cclrc.ac.uk\\Org\NLab\\Projects\\Vela\\Software\\VELA_CLARA_PYDs\\bin\\stage')
import VELA_CLARA_Scope_Control as vcsc

s = vcsc.init()
scopeController = s.physical_VELA_INJ_Scope_Controller()

scope_name = "WVF01"
channel_name = vcsc.SCOPE_PV_TYPE.TR3

while True:
	numShots = 10
	# scopeController.monitorATraceForNShots( scope_name, channel_name, numShots )
	scopeController.monitorTracesForNShots( numShots )
	while scopeController.isMonitoringScopeTrace(scope_name,channel_name):
		time.sleep(0.01)

	print '*** Acquired 100 shots ***'	
	
	baseline_start = 1
	baseline_end = 100
	baseline_area = scopeController.getAreaUnderPartOfTrace( scope_name, channel_name, baseline_start, baseline_end )
	mean_baseline_area = numpy.mean( baseline_area )
	
	dc_start = 200
	dc_end = 1600
	dc_area = scopeController.getAreaUnderPartOfTrace( scope_name, channel_name, dc_start, dc_end )
	mean_dc_area = numpy.mean( dc_area )
	
	print 'Mean baseline measurement = ', mean_baseline_area*numpy.float64(10^9), ' nC'
	print 'Mean dark current measurement = ', mean_dc_area*numpy.float64(10^9), ' nC'
	print 'With baseline subtracted = ', mean_dc_area*numpy.float64(10^9) - mean_baseline_area*numpy.float64(10^9), ' nC'
		
	time.sleep(0.1)
