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
        screenController(const bool show_messages,
                         const bool show_debug_messages,
                         const std::string & conf1,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const VELA_ENUM::MACHINE_AREA);
        ~screenController();

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING
        virtual std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        virtual std::map< VELA_ENUM::ILOCK_NUMBER, std::string >         getILockStatesStr( const std::string & name );

        virtual double get_CA_PEND_IO_TIMEOUT();
        virtual void   set_CA_PEND_IO_TIMEOUT( double val );

        /// GETTERS
        bool isHOut(const std::string & name);
        bool isVOut(const std::string & name);
        bool is_HandV_OUT(const std::string & name);
        bool isHIn(const std::string & name);
        bool isVIn(const std::string & name);
        bool isScreenIn(const std::string & name);
        bool setScreenSDEV(const std::string & name, const screenStructs::SCREEN_STATE & state );
        bool setScreenTrigger(const std::string & name );
//        screenStructs::SCREEN_STATE getScreenState( const std::string & name, const bool weKnowEntryExists = false );
        const std::string getScreenState( const std::string & name );
        const screenStructs::screenObject & getScreenObject(const std::string & name);
        bool isScreenInState(const std::string & name, screenStructs::SCREEN_STATE sta);
        bool isYAGIn(const std::string & name);
        bool isScreenIN(const std::string & name, const bool weKnowEntryExists = false );
        bool isHElement(const std::string & name, const screenStructs::SCREEN_STATE e);
        bool isVElement(const std::string & name, const screenStructs::SCREEN_STATE e);
        bool isHEnabled(const std::string & name);
        bool isVEnabled(const std::string & name);
        double getACTPOS(const std::string & name);
        double getDevCent(const std::string & name);
        /// SETTERS
        void moveScreenTo( const std::string & name, const screenStructs::SCREEN_STATE & state );
        void resetPosition( const std::string & name );
        void jogScreen( const std::string & name, const double jog );
        void setPosition( const std::string & name, const double setPos );
        bool setEX( const std::string & name );

//  // this is the main move funciton, all higher level versions (i.e. screenIN) end up here, this does all the hard work / logic
//        bool screenMoveTo( const std::vector< std::string > & names, const std::vector< screenStructs::SCREEN_STATE > & states);
//        bool screenMoveTo( const std::string & name, const screenStructs::SCREEN_STATE & states);
//        bool screenIN ( const std::string & name );
//        bool screenOUT( const std::string & name );
//        bool screenOUT( const std::vector< std::string > & names );
//        bool screenIN ( const std::vector< std::string > & names );
//        bool setPosition(const std::string & name, const screenStructs::DRIVER_DIRECTION dir, const double value );
//
//        double get_H_ACTPOS( const std::string & name );
//        double get_V_ACTPOS( const std::string & name );
//
//        // 'existential quantification' - ahem
//        bool isMoving(const std::string& name);
//        bool isNotMoving(const std::string& name);
//        bool is_VELA_PNEUMATIC(const std::string & name);
//        bool is_CLARA_PNEUMATIC(const std::string & name);
//        bool is_VELA_HV_MOVER (const std::string & name);
//        bool is_CLARA_HV_MOVER (const std::string & name);
//        bool is_CLARA_V_MOVER (const std::string & name);
//        bool isScreenOUT(const std::string & name);
//        bool isScreenIN(const std::string & name);
//        std::vector<bool> isScreenIN(const std::vector<std::string> & name );
//        std::vector<bool> isScreenOUT(const std::vector<std::string> & name );
//        std::vector<bool> exists_and_isLocked(const std::string& name);
//        std::vector<bool> exists_and_isNotLocked(const std::string& name);
//
//        const std::vector<std::string> get_VELA_PNEUMATIC_Screens(const std::vector< std::string > & names );
//        const std::vector<std::string> get_CLARA_PNEUMATIC_Screens(const std::vector< std::string > & names );
//        const std::vector<std::string> get_VELA_HV_MOVER_Screens( const std::vector< std::string > & names );
//        const std::vector<std::string> get_CLARA_HV_MOVER_Screens( const std::vector< std::string > & names );
//        const std::vector<std::string> get_CLARA_V_MOVER_Screens( const std::vector< std::string > & names );
//
//        // i want to make sure so python does not have access to the second arguemnt
//        screenStructs::SCREEN_STATE getScreenState(const std::string & name);

#ifdef BUILD_DLL
        boost::python::dict getILockStatesStr_Py(std::string& name);
        boost::python::dict getILockStates_Py   (std::string& name);
        boost::python::dict get_SCREEN_STATE_Definition();
        boost::python::dict get_DRIVER_STATE_Definition();
#endif // BUILD_DLL

        // YOU CANNOT SET THE CONTROLLER TYPE, NO-WAY JOSE
        VELA_ENUM::MACHINE_AREA getmyMachineArea(){return myMachineArea;};
        // void setmyMachineArea(){ no chance matey };

    private:
        void initialise();
        ///No singletons, no pointers, let's just have an object
        screenInterface localInterface;
        std::vector< std::string > ScreenNames;
        bool shouldStartEPICs, startVirtualMachine;
        const VELA_ENUM::MACHINE_AREA myMachineArea;

};
#endif //
