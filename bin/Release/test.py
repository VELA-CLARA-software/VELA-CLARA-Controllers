import VC_MagnetControl as mag

vc = mag.VCmagnets()

a  = vc.virtual_VELA_INJ_Magnet_Controller_NOEPICS();

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

