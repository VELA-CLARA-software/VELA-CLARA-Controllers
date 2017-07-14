#ifndef L01MODCONTROLLER_H
#define L01MODCONTROLLER_H

#include "controller.h"
#include "l01ModInterface.h"


class l01ModController : public controller
{
    public:
        l01ModController();
        l01ModController(const bool show_messages, const bool show_debug_messagese,
                         const std::string& l01ModConf, const bool startVirtualMachine,
                         const bool shouldStartEPICs);
        ~l01ModController();

        const rfModStructs::l01ModObject& getObjConstRef();
        bool reset();




        // These are pure virtual methods, so need to have some implmentation in derived classes
        virtual double get_CA_PEND_IO_TIMEOUT();
        virtual void   set_CA_PEND_IO_TIMEOUT(double val);
        std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::ILOCK_STATE> getILockStates(const std::string& name);
        std::map<VELA_ENUM::ILOCK_NUMBER,std::string> getILockStatesStr(const std::string& name);
    protected:

    private:



        l01ModInterface localInterface;
        const bool shouldStartEPICs;

};

#endif // L01MODCONTROLLER_H
