#ifndef VCMAGNETS_H
#define VCMAGNETS_H

//
#include "magnetController.h"



class VCmagnets
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for expliocit functions asa quick fix
    public:
        VCmagnets();
        ~VCmagnets();

        magnetController& virtual_VELA_INJ_Magnet_Controller();
        magnetController& offline_VELA_INJ_Magnet_Controller();
        magnetController& physical_VELA_INJ_Magnet_Controller();

        magnetController& virtual_VELA_BA1_Magnet_Controller();
        magnetController& offline_VELA_BA1_Magnet_Controller();
        magnetController& physical_VELA_BA1_Magnet_Controller();

        magnetController& virtual_VELA_BA2_Magnet_Controller();
        magnetController& offline_VELA_BA2_Magnet_Controller();
        magnetController& physical_VELA_BA2_Magnet_Controller();

        magnetController& virtual_CLARA_PH1_Magnet_Controller();
        magnetController& offline_CLARA_PH1_Magnet_Controller();
        magnetController& physical_CLARA_PH1_Magnet_Controller();

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        magnetController& getMagnetController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area );

    protected:

    private:

     /// we just need to decide good names for these things...
        magnetController * virtual_VELA_INJ_Magnet_Controller_Obj;
        magnetController * offline_VELA_INJ_Magnet_Controller_Obj;
        magnetController * physical_VELA_INJ_Magnet_Controller_Obj;

        magnetController * virtual_VELA_BA1_Magnet_Controller_Obj;
        magnetController * offline_VELA_BA1_Magnet_Controller_Obj;
        magnetController * physical_VELA_BA1_Magnet_Controller_Obj;

        magnetController * virtual_VELA_BA2_Magnet_Controller_Obj;
        magnetController * offline_VELA_BA2_Magnet_Controller_Obj;
        magnetController * physical_VELA_BA2_Magnet_Controller_Obj;

        magnetController * virtual_CLARA_PH1_Magnet_Controller_Obj;
        magnetController * offline_CLARA_PH1_Magnet_Controller_Obj;
        magnetController * physical_CLARA_PH1_Magnet_Controller_Obj;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;

        const VELA_ENUM::MACHINE_AREA  VELA_INJ,VELA_BA1,VELA_BA2,CLARA_PH1,UNKNOWN_AREA;
};


#endif // VCMAGNETS_H
