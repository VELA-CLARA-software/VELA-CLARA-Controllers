#ifndef L01MODINTERFACE_H
#define L01MODINTERFACE_H
// djs
#include "interface.h"
#include "rfModStructs.h"
#include "structs.h"
#include "l01ModConfigReader.h"


#include <string>

class l01ModInterface : public interface
{
    public:
        l01ModInterface(const std::string &l01ModConf,
                                const bool startVirtualMachine,
                                const bool& show_messages_ptr, const bool& show_debug_messages_ptr,
                                const bool shouldStartEPICs);
        virtual ~l01ModInterface();

        const rfModStructs::l01ModObject& getObjConstRef();

        bool reset();

        bool setOff();

//
        typedef std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER,std::string> IlockMap2;
        // These are pure virtual methods, so need to have some implmentation in derived classes
        IlockMap1 getILockStates(const std::string & name ){ IlockMap1 r;return r; }
        IlockMap2 getILockStatesStr(const std::string & name){ IlockMap2 r;return r; }
    protected:

    private:

        l01ModConfigReader configReader; /// class member so we can pass in file path in ctor

        //const bool shouldStartEPICs;
        bool allChidsInitialised;
        void initialise();

        void initChids();
        void addChannel(const std::string & pvRoot, rfModStructs::l01_pvStruct& pv);
        void startMonitors();

        void updateErrorWordList(const int part, const char* value);
        void updateErrorDescList(const int part, const char* value);

        std::vector<rfModStructs::l01_monitorStruct*> continuousMonitorStructs;

        static void staticEntryL01ModMonitor(const event_handler_args args);

        void updateSystemStatePut(const event_handler_args args);

        rfModStructs::l01ModObject l01Mod;

        void updateHoldRFOnState(const event_handler_args& args);


        void updateL01_StateRead(const event_handler_args args);
        void updateL01_Fault(const event_handler_args args);

        void killMonitor(rfModStructs::l01_monitorStruct* ms);

        bool setModState(const rfModStructs::L01_MOD_STATE v)const;
};
#endif // L01MODINTERFACE_H
