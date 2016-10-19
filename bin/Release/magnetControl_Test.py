import VELA_CLARA_MagnetControl as mag

vc = mag.init()

a  = vc.virtual_VELA_INJ_Magnet_Controller();

b = a.getMagnetNames()

for i in b:
    print i
    
d = a.getMagObjConstRef("QUAD01")

print''
print 'QUAD 01 Data'
print 'magnet name = ', d.name
print 'magnet type = ', d.magType
print 'siWithPol   = ', d.siWithPol
print 'riWithPol   = ', d.riWithPol
print 'd.nPSU.psuState   = ', d.nPSU.psuState
print 'd.rPSU.psuState   = ', d.rPSU.psuState

