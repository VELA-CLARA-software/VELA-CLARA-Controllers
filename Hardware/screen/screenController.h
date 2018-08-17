#ifndef CLARA_SCREEN_CONTROLLER_H
#define CLARA_SCREEN_CONTROLLER_H
//project
#include "screenStructs.h"
#include "screenInterface.h"
#include "controller.h"
#include "structs.h"
//stl
#include <string>
#include <vector>

class screenController : public controller
{
    public:
        screenController();
        screenController(const std::string & conf1,
                         bool& show_messages,
                         bool& show_debug_messages,
                         const bool shouldStartEPICs,
                         const bool startVirtualMachine,
                         const HWC_ENUM::MACHINE_AREA);
        ~screenController();

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING
        virtual std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > getILockStates( const std::string & name )const;
        virtual std::map< HWC_ENUM::ILOCK_NUMBER, std::string >         getILockStatesStr( const std::string & name )const;

        virtual double get_CA_PEND_IO_TIMEOUT()const;
        virtual void   set_CA_PEND_IO_TIMEOUT( double val );

        /// GETTERS
        bool isHOut(const std::string & name);
        bool isVOut(const std::string & name);
        bool is_HandV_OUT(const std::string & name);
        bool isHIn(const std::string & name);
        bool isVIn(const std::string & name);
        bool isMover(const std::string & name);
        bool isPneumatic(const std::string & name);
        bool isScreenIn(const std::string & name);
        bool isHMoving(const std::string & name);
        bool isVMoving(const std::string & name);
        bool isScreenMoving(const std::string & name);
//        screenStructs::SCREEN_STATE getScreenState( const std::string & name, const bool weKnowEntryExists = false );
        const std::string getScreenStateStr( const std::string & name );
        const screenStructs::SCREEN_STATE getScreenState( const std::string & name );
        const std::string getScreenType(const std::string & name);
        const screenStructs::screenObject & getScreenObject(const std::string & name);
        std::vector< screenStructs::SCREEN_STATE > getAvailableDevices(const std::string & name);
        std::vector< std::string > getScreenNames();
//        std::vector< bool > isScreenIn(const std::vector<std::string> & name );
        bool isScreenInState(const std::string & name, screenStructs::SCREEN_STATE sta);
        bool isYAGIn(const std::string & name);
        bool isHElement(const std::string & name, const screenStructs::SCREEN_STATE e);
        bool isVElement(const std::string & name, const screenStructs::SCREEN_STATE e);
        bool isPElement(const std::string & name, const screenStructs::SCREEN_STATE e);
        bool isHEnabled(const std::string & name);
        bool isVEnabled(const std::string & name);

        bool isClearForBeam(const std::string & name);

        double getACTPOS(const std::string & name);
        double getDevCent(const std::string & name);
        double getJDiff(const std::string & name);
        double getDevicePosition(const std::string & name, const screenStructs::SCREEN_STATE state);
        double getPosition(const std::string & name);
        //boost::python::list getScreenNames_Py();
        /// SETTERS
        void moveScreenTo( const std::string & name, const screenStructs::SCREEN_STATE & state );
        void insertYAG( const std::string & name );
        void moveScreenOut( const std::string & name );
        void resetPosition( const std::string & name );
        void jogScreen( const std::string & name, const double jog );
        void setPosition( const std::string & name, const double setPos );
        bool setEX( const std::string & name );
        bool setScreenSDEV(const std::string & name, const screenStructs::SCREEN_STATE & state );
        bool setScreenTrigger(const std::string & name );

#ifdef BUILD_DLL
        boost::python::list getScreenNames_Py();
//        boost::python::list isScreenIn_Py(const std::vector<std::string> & name);
        boost::python::dict getILockStatesStr_Py(std::string& name);
        boost::python::dict getILockStates_Py   (std::string& name);
        boost::python::dict get_SCREEN_STATE_Definition();
        boost::python::dict get_DRIVER_STATE_Definition();
#endif // BUILD_DLL

    private:
        void initialise();
        ///No singletons, no pointers, let's just have an object
        screenInterface localInterface;
        std::vector< std::string > ScreenNames;
        const bool shouldStartEPICs;
        const HWC_ENUM::MACHINE_AREA machineArea;

};
#endif //
