import VELA_CLARA_Camera_Control as tim


h = tim.init()
h.setVerbose()
cameras = h.offline_CLARA_Camera_Controller()
vc = cameras.getCamObjConstRef('VC')
print 'Selected camera name',cameras.cameraName()
cameras.setCamera('CAM01')
print 'Selected camera name',cameras.cameraName()
