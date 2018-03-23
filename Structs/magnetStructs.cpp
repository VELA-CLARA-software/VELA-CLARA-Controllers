#include "magnetStructs.h"
#include "magnetInterface.h"
//#include <iostream>
//______________________________________________________________________________
bool magnetStructs::magnetObject::setSI(const double v)
{
    return interface->setSI(name,v);
}
//______________________________________________________________________________
bool magnetStructs::magnetObject::setPSU(const MAG_PSU_STATE s)
{
    switch(s)
    {
        case magnetStructs::MAG_PSU_STATE::ON:
            return interface->switchONpsu(name);
        case magnetStructs::MAG_PSU_STATE::OFF:
            return interface->switchOFFpsu(name);
    }
    return false;
}
