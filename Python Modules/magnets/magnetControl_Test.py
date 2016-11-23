import sys,os

sys.path.append('\\\\fed.cclrc.ac.uk\\org\NLab\\ASTeC\\Projects\\VELA\\Software\\VELA_CLARA_PYDs\\bin\\Stage\\')


from VELA_CLARA_MagnetControl import MAG_PSU_STATE, MAG_TYPE, MAG_REV_TYPE
raw_input('1')

print MAG_PSU_STATE.MAG_PSU_ON

raw_input('wait')
from VELA_CLARA_MagnetControl import MACHINE_MODE, MACHINE_AREA

print MACHINE_MODE.OFFLINE 

raw_input('wait')

import VELA_CLARA_MagnetControl as mag

vc = mag.init()

#a  = vc.virtual_VELA_INJ_Magnet_Controller();

c  = vc.getMagnetController( MACHINE_MODE.OFFLINE, MACHINE_AREA.VELA_INJ)

c.setSI("QUAD01",-3.4)

raw_input('wait')

print "QUAD01 SI = "
print c.getSI("QUAD01")

raw_input('wait')

b = c.getMagnetNames()

raw_input()

for i in b:
    print i
    
d = c.getMagObjConstRef("SOL")

print''
print 'SOL Data'
print 'magnet name = ', d.name
print 'magnet type = ', d.magType
print 'siWithPol   = ', d.siWithPol
print 'riWithPol   = ', d.riWithPol
print 'riTolerance   = ', d.riTolerance
print 'd.nPSU.psuState   = ', d.nPSU.psuState
print 'd.rPSU.psuState   = ', d.rPSU.psuState
print 'd.position   = ', d.position
print 'd.slope   = ', d.slope
print 'd.intercept   = ', d.intercept
print ''
print ''


d = c.getMagObjConstRef("HCOR01")

print''
print 'HCOR01 Data'
print 'magnet name = ', d.name
print 'magnet type = ', d.magType
print 'siWithPol   = ', d.siWithPol
print 'riWithPol   = ', d.riWithPol
print 'riTolerance   = ', d.riTolerance
print 'd.nPSU.psuState   = ', d.nPSU.psuState
print 'd.rPSU.psuState   = ', d.rPSU.psuState
print 'd.position   = ', d.position
print 'd.slope   = ', d.slope
print 'd.intercept   = ', d.intercept
print 'd.manufacturer   = ', d.manufacturer
print 'd.serialNumber   = ', d.serialNumber
print 'd.measurementDataLocation   = ', d.measurementDataLocation
print 'd.magneticLength   = ', d.magneticLength


raw_input()
