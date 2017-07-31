import VELA_CLARA_Camera_DAQ_Control as tim

import time

h = tim.init()
h.setVerbose()
cameras = h.physical_CLARA_Camera_DAQ_Controller()

test = cameras.getCamDAQObjConstRef('VC')

sel=cameras.getSelectedDAQRef()
sel2 = cameras.getSelectedDAQPtr()
VC = cameras.getVCDAQRef()

print 'Default Selected camera ',sel.name
suc = cameras.setCamera('CAM01')
print 'Ref to Selected camera name ',sel.name
#print 'Ref to Selected camera state ',sel.state
print 'Function to Selected camera state', cameras.isAquiring('CAM01')
sel = cameras.getSelectedDAQRef()

#cameras.setCamera('VC')
#print 'Selected camera name ',cameras.selectedCamera()
print 'Ref Selected camera name after being redefined is ',sel.name



