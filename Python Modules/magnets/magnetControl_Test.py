import sys,os

sys.path.append('\\\\fed.cclrc.ac.uk\\org\NLab\\ASTeC\\Projects\\VELA\\Software\\VELA_CLARA_PYDs\\bin\\Stage\\')


from VELA_CLARA_MagnetControl import MAG_PSU_STATE, MAG_TYPE, MAG_REV_TYPE
raw_input('1')

print MAG_PSU_STATE.MAG_PSU_ON

raw_input('2')
from VELA_CLARA_MagnetControl import MACHINE_MODE, MACHINE_AREA

print MACHINE_MODE.OFFLINE 

raw_input('2')

import VELA_CLARA_MagnetControl as mag

vc = mag.init()

#a  = vc.virtual_VELA_INJ_Magnet_Controller();



c  = vc.getMagnetController(MACHINE_MODE.OFFLINE, MACHINE_AREA.VELA_INJ);

raw_input('2')

b = c.getMagnetNames()

raw_input()

for i in b:
    print i
    
d = c.getMagObjConstRef("QUAD01")

raw_input()

print''
print 'QUAD 01 Data'
print 'magnet name = ', d.name
print 'magnet type = ', d.magType
print 'siWithPol   = ', d.siWithPol
print 'riWithPol   = ', d.riWithPol
print 'd.nPSU.psuState   = ', d.nPSU.psuState
print 'd.rPSU.psuState   = ', d.rPSU.psuState

raw_input()
