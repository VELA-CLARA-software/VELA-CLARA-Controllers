import VELA_CLARA_Camera_DAQ_Control as tim


h = tim.init()
h.setVerbose()
cameras = h.offline_CLARA_Camera_DAQ_Controller()
print 'hi'
vc = cameras.getCamDAQObjConstRef('VC')
#print 'Selected camera name',cameras.selectedCamera()
#cameras.setCamera('CAM01')
#print 'Selected camera name',cameras.selectedCamera()

