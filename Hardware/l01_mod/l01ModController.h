#ifndef L01MODCONTROLLER_H
#define L01MODCONTROLLER_H

#include "controller.h"
#include "l01ModInterface.h"


class l01ModController : public controller
{
    public:
        l01ModController();
        l01ModController(bool& show_messages, bool& show_debug_messagese,
                         const std::string& l01ModConf, const bool startVirtualMachine,
                         const bool shouldStartEPICs);
        ~l01ModController();

        const rfModStructs::l01ModObject& getObjConstRef();
        bool reset();
        bool setOff();

        // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val);
        std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> getILockStates(const std::string& name);
        std::map<HWC_ENUM::ILOCK_NUMBER,std::string> getILockStatesStr(const std::string& name);
    protected:

    private:

        l01ModInterface localInterface;
        //const bool shouldStartEPICs;

};

#endif // L01MODCONTROLLER_H
