#include "VCgeneralMonitor.h"
#include <iostream>
#include <iomanip>
#include <epicsTime.h>

VCgeneralMonitor::VCgeneralMonitor(const bool shouldShowMessage,const bool  shouldShowDebugMessage ):
controller( shouldShowMessage, shouldShowDebugMessage ),
pvMonitorMapCount(UTL::ZERO_INT),
defaultCOUNT(1),
defaultMASK(DBE_VALUE),
stringPrefix("t"),
intPrefix("i"),
shortPrefix("h"),
floatPrefix("f"),
enumPrefix("e"),
charPrefix("c"),
longPrefix("l"),
doublePrefix("d"),
ca_chid_successmess("Successfully found ChId."),
ca_chid_failuremess("!!TIMEOUT!! ChIds NOT found."),
ca_subs_successmess("Succesfully subscribed, now monitoring."),
ca_subs_failuremess("!!TIMEOUT!! Subscription to monitor failed."),
ca_create_channel_str("ca_create_channel"),
ca_create_subscription_str("ca_create_subscription"),
ca_clear_subscription_str("ca_clear_subscription"),
ca_csub_successmess("Succesfully un-subscribed, not monitoring."),
ca_csub_failuremess("!!TIMEOUT!! un-subscription to monitor failed."),
ca_clear_channel_str("ca_clear_channel"),
ca_cchi_successmess("Succesfully cleared ChId."),
ca_cchi_failuremess("!!TIMEOUT!! clearing ChID failed."),
returnFail("FAILED"),
CA_PEND_IO_TIMEOUT(2.0)
{
    // This 'enables' callbacks, monitoring, etc
    ca_context_create( ca_enable_preemptive_callback );
    // This is the current (AND ONLY) context,
    // use it to join from new threads with ca_attach_context
    // you need to attach to this context if multi-threading
    thisCaContext = ca_current_context();
}
//______________________________________________________________________________
void VCgeneralMonitor::setQuiet()
{
    std::cout << "VCgeneralMonitor Quiet Mode Set." << std::endl;
    SHOW_DEBUG_MESSAGES = false;
    SHOW_MESSAGES = false;
}
//______________________________________________________________________________
void VCgeneralMonitor::setVerbose()
{
    std::cout << "VCgeneralMonitor Verbose Mode Set." << std::endl;
    SHOW_DEBUG_MESSAGES = true;
    SHOW_MESSAGES = true;
}
//______________________________________________________________________________
void VCgeneralMonitor::setMessage()
{
    std::cout << "VCgeneralMonitor Message Mode Set." << std::endl;
    SHOW_DEBUG_MESSAGES = false;
    SHOW_MESSAGES = true;
}
//______________________________________________________________________________
void VCgeneralMonitor::setDebugMessage()
{
    std::cout << "VCgeneralMonitor DebugMessage Mode Set." << std::endl;
    SHOW_DEBUG_MESSAGES = true;
    SHOW_MESSAGES      = false;
}
//______________________________________________________________________________
VCgeneralMonitor::~VCgeneralMonitor()
{

}
//______________________________________________________________________________
//template<typename T>
boost::python::object VCgeneralMonitor::getValue(const std::string & id )
{
    gmStructs::pvStruct* pvs = nullptr;
//    success = false;
    if(isIntPV(id) )
    {
        if(entryExists(intPVMap, id) )
        {
            //message("getPVStruct ",  id, " isIntPV");
             return object(intPVMap[id].data[0]->v);
            //message("getPVStruct found ",  id, " in intPVMap");
//            success = true;
        }
    }
    else if(isFloatPV(id) )
    {
        if(entryExists(floatPVMap, id) )
        {
            //message("getPVStruct ",  id, " isFloatPV");
            return object(floatPVMap[id].data[0]->v);
        }
    }
    else if(isEnumPV(id) )
    {
        if(entryExists(enumPVMap, id) )
        {
            //message("getPVStruct ",  id, " isEnumPV");
            return object(enumPVMap[id].data[0]->v);
        }
    }
    else if(isCharPV(id) )
    {
        if( entryExists(charPVMap, id) )
        {
            //message("getPVStruct ",  id, " isCharPV");
            return object(charPVMap[id].data[0]->v);
        }
    }
    else if(isLongPV(id) )
    {
        if(entryExists(longPVMap, id) )
        {
            //message("getPVStruct ",  id, " isLongPV");
            return object(longPVMap[id].data[0]->v);
        }
    }
    else if(isDoublePV(id) )
    {
        if(entryExists(doublePVMap, id) )
        {
            //message("getPVStruct ",  id, " isDoublePV");
            return object(doublePVMap[id].data[0]->v);
        }
    }
    return object();
}
//______________________________________________________________________________
//template<typename T>
//T VCgeneralMonitor::getValue(const std::string & id )
//{
//    gmStructs::pvStruct* pvs = nullptr;
////    success = false;
//    T r;
//    if(isIntPV(id) )
//    {
//        message("getPVStruct ",  id, " isIntPV");
//        if( entryExists(intPVMap, id) )
//        {
//             r = intPVMap[id].data[0]->v;
//            //message("getPVStruct found ",  id, " in intPVMap");
////            success = true;
//        }
//    }
//    else if(isFloatPV(id) )
//    {
//        message("getPVStruct ",  id, " isFloatPV");
//
//        if( entryExists(floatPVMap, id) )
//        {
//            r = floatPVMap[id].data[0]->v;
//        }
//    }
//    else if(isEnumPV(id) )
//    {
//        message("getPVStruct ",  id, " isEnumPV");
//
//        if( entryExists(enumPVMap, id) )
//        {
//            r = enumPVMap[id].data[0]->v;
//        }
//    }
//    else if(isCharPV(id) )
//    {
//        message("getPVStruct ",  id, " isCharPV");
//
//        if( entryExists(charPVMap, id) )
//        {
//            r = charPVMap[id].data[0]->v;
//        }
//    }
//    else if(isLongPV(id) )
//    {
//        message("getPVStruct ",  id, " isLongPV");
//
//        if( entryExists(longPVMap, id) )
//        {
//            r = longPVMap[id].data[0]->v;
//        }
//    }
//    else if(isDoublePV(id) )
//    {
//        message("getPVStruct ",  id, " isDoublePV");
//
//        if( entryExists(doublePVMap, id) )
//        {
//            r = doublePVMap[id].data[0]->v;
//        }
//    }
//    return r;
//}
//______________________________________________________________________________
std::string VCgeneralMonitor::connectPV(const std::string & pvFullName,const std::string & pvType )
{
    std::string r = returnFail;
    //chtype tempchtype;
    //std::string id;
    debugMessage("Getting pvType for ", pvType);
    gmStructs::pvStruct* pvs =  getCHTYPEandPrefix(pvType);
    if(pvs)
    {// we recognise the PV channel type
        pvs->pvFullName = pvFullName;
        pvs->COUNT= defaultCOUNT;
        pvs->MASK = defaultMASK;
        debugMessage(pvType, " found. Setting up channel with id ",pvs->id, " to, ", pvFullName);
        bool shouldCarryOn = setUpChannel(*pvs);
        if(shouldCarryOn)
        {
            shouldCarryOn = setupMonitor(*pvs);
            if( shouldCarryOn )
            {
                r = pvs->id;
            }
        }
    }
    if(r == returnFail)
    {
        message("connectPV FAILED");
        disconnectPV(pvs->id);
    }
    else
    {
        ++pvMonitorMapCount;
    }
    return r;
}
//______________________________________________________________________________
void VCgeneralMonitor::staticEntryMonitor(const event_handler_args args)
{
    gmStructs::monitorStruct* ms = static_cast<gmStructs::monitorStruct *>(args.usr);
    ms->gm->updateValue( ms->id, args );
}
//______________________________________________________________________________
void VCgeneralMonitor::updateValue(const std::string & id,const event_handler_args& args)
{
    if(args.status != ECA_NORMAL)
    {
        std::cout<<"please god, no, never show this... "<<std::endl;
    }
    switch( args.type)
    {
        case  DBR_DOUBLE:
            if( doublePVMap[id].data.size() == 0 )
            {
                doublePVMap[id].data.push_back(new gmStructs::dataEntry<double>() );
            }
            doublePVMap[id].data[0]->v = *(double*)args.dbr;// MAGIC_NUMBER
            //doublePVMap[id].data[0]->t = p->stamp;// MAGIC_NUMBER
            break;
        case  DBR_INT:
            if( intPVMap[id].data.size() == 0 )
            {
                intPVMap[id].data.push_back(new gmStructs::dataEntry<int>() );
            }
            intPVMap[id].data[0]->v = *(int*)args.dbr;// MAGIC_NUMBER
            break;
    }

//        if(isStringPV(id))
//        {
//            if( stringPVMap.at(id).data.size() == 0 )
//                stringPVMap.at(id).data.pushBack( dataEntry<std::string>() );
//            else
//            {
//                stringPVMap.at(id).data.v = *(std::string*)dbr;// this probably doesn't work and why do strings?
//            }
//        }
//    if(isIntPV(id))
//    {
//        // see db_access.h
//        const dbr_time_short * p = (const struct dbr_time_short*)dbr;
//        if( intPVMap.at(id).data.size() == 0 )
//        {
//            intPVMap.at(id).data.push_back( new gmStructs::dataEntry<int>() );
//        }
//        intPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
//        intPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
//
//    }
//    else if(isFloatPV(id))
//    {
//        // see db_access.h
//        const dbr_time_float * p = (const struct dbr_time_float*)dbr;
//        if( floatPVMap.at(id).data.size() == 0 )
//        {
//            floatPVMap.at(id).data.push_back( new gmStructs::dataEntry<float>() );
//        }
//        floatPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
//        floatPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
//    }
//    else if(isEnumPV(id))
//    {
//        // see db_access.h
//        const dbr_time_enum * p = (const struct dbr_time_enum*)dbr;
//        if( enumPVMap.at(id).data.size() == 0 )
//        {
//            enumPVMap.at(id).data.push_back( new gmStructs::dataEntry<unsigned short>() );
//        }
//        enumPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
//        enumPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
//    }
//    else if(isCharPV(id))
//    {
//        // see db_access.h
//        const dbr_time_char * p = (const struct dbr_time_char*)dbr;
//        if( charPVMap.at(id).data.size() == 0 )
//        {
//            charPVMap.at(id).data.push_back( new gmStructs::dataEntry<char>() );
//        }
//        charPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
//        charPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
//    }
//    else if(isLongPV(id))
//    {
//        // see db_access.h
//        const dbr_time_long * p = (const struct dbr_time_long*)dbr;
//        if( longPVMap.at(id).data.size() == 0 )
//        {
//            longPVMap.at(id).data.push_back( new gmStructs::dataEntry<long>() );
//        }
//        longPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
//        longPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
//    }
//    else if(isDoublePV(id))
//    {
//        // see db_access.h
//        const dbr_time_double* p = (const struct dbr_time_double*)dbr;
//        if( longPVMap.at(id).data.size() == 0 )
//        {
//            doublePVMap.at(id).data.push_back( new gmStructs::dataEntry<double>() );
//        }
//        doublePVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
//        doublePVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
//
//        message("updated, value = ", doublePVMap.at(id).data[0]->v);// MAGIC_NUMBER
//        printTimeStamp( doublePVMap.at(id).data[0]->t );
//    }
}
//______________________________________________________________________________
void VCgeneralMonitor::updateTimeAndValue(const std::string & id,const  void * dbr)
{
//        if(isStringPV(id))
//        {
//            if( stringPVMap.at(id).data.size() == 0 )
//                stringPVMap.at(id).data.pushBack( dataEntry<std::string>() );
//            else
//            {
//                stringPVMap.at(id).data.v = *(std::string*)dbr;// this probably doesn't work and why do strings?
//            }
//        }
    if(isIntPV(id))
    {
        // see db_access.h
        const dbr_time_short * p = (const struct dbr_time_short*)dbr;
        if( intPVMap.at(id).data.size() == 0 )
        {
            intPVMap.at(id).data.push_back(new gmStructs::dataEntry<int>() );
        }
        intPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        intPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER

    }
    else if(isFloatPV(id))
    {
        // see db_access.h
        const dbr_time_float * p = (const struct dbr_time_float*)dbr;
        if( floatPVMap.at(id).data.size() == 0 )
        {
            floatPVMap.at(id).data.push_back( new gmStructs::dataEntry<float>() );
        }
        floatPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        floatPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
    }
    else if(isEnumPV(id))
    {
        // see db_access.h
        const dbr_time_enum * p = (const struct dbr_time_enum*)dbr;
        if( enumPVMap.at(id).data.size() == 0 )
        {
            enumPVMap.at(id).data.push_back( new gmStructs::dataEntry<unsigned short>() );
        }
        enumPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        enumPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
    }
    else if(isCharPV(id))
    {
        // see db_access.h
        const dbr_time_char * p = (const struct dbr_time_char*)dbr;
        if( charPVMap.at(id).data.size() == 0 )
        {
            charPVMap.at(id).data.push_back( new gmStructs::dataEntry<char>() );
        }
        charPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        charPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
    }
    else if(isLongPV(id))
    {
        // see db_access.h
        const dbr_time_long * p = (const struct dbr_time_long*)dbr;
        if( longPVMap.at(id).data.size() == 0 )
        {
            longPVMap.at(id).data.push_back( new gmStructs::dataEntry<long>() );
        }
        longPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        longPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
    }
    else if(isDoublePV(id))
    {
        // see db_access.h
        const dbr_time_double* p = (const struct dbr_time_double*)dbr;
        if( longPVMap.at(id).data.size() == 0 )
        {
            doublePVMap.at(id).data.push_back( new gmStructs::dataEntry<double>() );
        }
        doublePVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        doublePVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER

        debugMessage("updated, value = ", doublePVMap.at(id).data[0]->v);// MAGIC_NUMBER
        printTimeStamp( doublePVMap.at(id).data[0]->t );
    }
}
//______________________________________________________________________________
void VCgeneralMonitor::printTimeStamp( const epicsTimeStamp & stamp)
{// look in epicsTime.h
//    const dbr_time_char * pTD = ( const struct dbr_time_char * ) args.dbr;
//    epicsUInt32 sec  = pTD ->stamp.secPastEpoch;   /* seconds since 0000 Jan 1, 1990 */
//    epicsUInt32 nsec = pTD ->stamp.nsec;
    char timeString[36];//MAGIC_NUMBER
    epicsTimeToStrftime ( timeString, sizeof ( timeString ), "%a %b %d %Y %H:%M:%S.%f", &stamp );
//    const dbr_char_t * pValue = &pTD -> value;
    double val =  ( (double)stamp.nsec * 0.000000001 ) + (double)stamp.secPastEpoch;//MAGIC_NUMBER
    std::cout << std::setprecision(15) << std::showpoint<<   val << std::endl;
    std::string str = timeString;
    debugMessage("time string = ",str);// MAGIC_NUMBER
    debugMessage("time double = ",val);// MAGIC_NUMBER
    std::cout << std::setprecision(4) << std::showpoint<<   val << std::endl;
}
//______________________________________________________________________________
bool VCgeneralMonitor::setUpChannel(gmStructs::pvStruct& pvs)
{
    debugMessage("setUpChannel is connecting ", pvs.id, "  to ", pvs.pvFullName );
    ca_create_channel(pvs.pvFullName.c_str(), 0, 0, 0,&pvs.CHID);
    int status = sendToEpics(ca_create_channel_str,ca_chid_successmess,ca_chid_failuremess);
    if( status == ECA_TIMEOUT )
    {
        //removeChanel();
    }
    else if( status == ECA_NORMAL )
    {
        debugMessage(pvs.id, " channel created.");
        pvs.CHIDConnected = true;
    }
    return pvs.CHIDConnected;
}
//______________________________________________________________________________
bool VCgeneralMonitor::setupMonitor(gmStructs::pvStruct& pvs )
{
    continuousMonitorStructs.push_back( gmStructs::monitorStruct() );
    continuousMonitorStructs.back().gm   = this;
    continuousMonitorStructs.back().id = pvs.id;

    ca_create_subscription(pvs.CHTYPE, pvs.COUNT,pvs.CHID, pvs.MASK,
                           VCgeneralMonitor::staticEntryMonitor,
                           (void*)&continuousMonitorStructs.back(),
                           &pvs.EVID);

    int status = sendToEpics(ca_create_subscription_str,ca_subs_successmess,ca_subs_failuremess);
    if(status == ECA_TIMEOUT)
    {
        //removeChanel();
    }
    else if( status == ECA_NORMAL )
    {
        pvs.MonitorConnected = true;
        debugMessage(pvs.id, " subscription successfull.");

    }
    return pvs.MonitorConnected;
}
//______________________________________________________________________________
int VCgeneralMonitor::sendToEpics(const std::string& ca,const std::string& mess1,const std::string& mess2)
{
    int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
    SEVCHK(status, ca.c_str());
    printStatusResult(status,mess1.c_str(),mess2.c_str());
    return status;
}
//______________________________________________________________________________
void VCgeneralMonitor::printStatusResult( const int status, const char * success, const char* timeout)
{
    switch (status)
    {
        case ECA_NORMAL:
            if( strlen( success ) != 0 )//MAGIC_NUMBER
                debugMessage( success );
            break;
        case ECA_TIMEOUT:
            if( strlen( timeout ) != 0 )//MAGIC_NUMBER
                message( timeout );
            break;
        default:
            message("!!! Unexpected error while searching: ", ca_message( status ));
    }
}
//______________________________________________________________________________
//gmStructs::pvStruct* VCgeneralMonitor::getCHTYPEandPrefix(const std::string & pvFullName,const std::string & pvType, bool & success)
gmStructs::pvStruct* VCgeneralMonitor::getCHTYPEandPrefix(const std::string & pvType)
{    // cancer

    gmStructs::pvStruct* r = nullptr;
    if(pvType == UTL::DBR_INT_STR)
    {
        r =  addToIntPVMap();
    }
    else if(pvType == UTL::DBR_SHORT_STR)
    {
        r = addToIntPVMap();// short is an INT (i hope)
    }
    else if(pvType == UTL::DBR_FLOAT_STR)
    {
        r = addToFloatPVMap();
    }
    else if(pvType == UTL::DBR_ENUM_STR)
    {
        r = addToEnumPVMap();
    }
    else if(pvType == UTL::DBR_CHAR_STR)
    {
        r = addToCharPVMap();
    }
//    else if(pvType == UTL::DBR_INT_STR)
//    {
//        success = true;
//        r = DBR_TIME_INT;
//        id = intPrefix;
//    }
    else if(pvType == UTL::DBR_LONG_STR)
    {
        r = addToLongPVMap();
    }
    else if(pvType == UTL::DBR_DOUBLE_STR)
    {
        r = addToDoublePVMap();
    }
    // the rest should probably not be used...
//    else if(pvType == UTL::DBR_TIME_STRING_STR)
//    {
//        success = true;
//        r = DBR_TIME_STRING;
//    }
//    else if(pvType == UTL::DBR_TIME_INT_STR)
//    {
//        success = true;
//        r = DBR_TIME_INT;
//    }
//    else if(pvType == UTL::DBR_TIME_SHORT_STR)
//    {
//        success = true;
//        r = DBR_TIME_SHORT;
//    }
//    else if(pvType == UTL::DBR_TIME_FLOAT_STR)
//    {
//        success = true;
//        r = DBR_TIME_FLOAT;
//    }
//    else if(pvType == UTL::DBR_TIME_ENUM_STR)
//    {
//        success = true;
//        r = DBR_TIME_ENUM;
//    }
//    else if(pvType == UTL::DBR_TIME_CHAR_STR)
//    {
//        success = true;
//        r = DBR_TIME_CHAR;
//    }
//    else if(pvType == UTL::DBR_TIME_LONG_STR)
//    {
//        success = true;
//        r = DBR_TIME_LONG_STR;
//    }
//    else if(pvType == UTL::DBR_TIME_DOUBLE_STR)
//    {
//        success = true;
//        r = DBR_TIME_DOUBLE;
//    }

    return r;
}
//______________________________________________________________________________
bool VCgeneralMonitor::disconnectPV(const std::string & id )
{
    gmStructs::pvStruct* pvs = getPVStruct_Ptr(id);
    if( pvs )
    {
        if(pvs->MonitorConnected)
        {
            killMonitor(*pvs);
        }
        if(pvs->CHIDConnected)
        {
            killChannel(*pvs);
        }
        return deleteID(id);
    }
    return false;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::getPVStruct_Ptr(const std::string& id )
{
    gmStructs::pvStruct* pvs = nullptr;
//    success = false;
    if(isIntPV(id) )
    {
        //message("getPVStruct ",  id, " isIntPV");
        if( entryExists(intPVMap, id) )
        {
            pvs = &intPVMap[id].pvs;
            //message("getPVStruct found ",  id, " in intPVMap");
//            success = true;
        }
    }
    else if(isFloatPV(id) )
    {
        //message("getPVStruct ",  id, " isFloatPV");

        if( entryExists(floatPVMap, id) )
        {
            pvs = &floatPVMap[id].pvs;
            //message("getPVStruct found ",  id, " in floatPVMap");
//            success = true;
        }
    }
    else if(isEnumPV(id) )
    {
        //message("getPVStruct ",  id, " isEnumPV");

        if( entryExists(enumPVMap, id) )
        {
            pvs = &enumPVMap[id].pvs;
            //message("getPVStruct found ",  id, " in enumPVMap");
//            success = true;
        }
    }
    else if(isCharPV(id) )
    {
        //message("getPVStruct ",  id, " isCharPV");

        if( entryExists(charPVMap, id) )
        {
            pvs = &charPVMap[id].pvs;
            //message("getPVStruct found ",  id, " in charPVMap");
//            success = true;
        }
    }
    else if(isLongPV(id) )
    {
        //message("getPVStruct ",  id, " isLongPV");

        if( entryExists(longPVMap, id) )
        {
            pvs = &longPVMap[id].pvs;
//            message("getPVStruct found ",  id, " in longPVMap");
//            success = true;
        }
    }
    else if(isDoublePV(id) )
    {
        //message("getPVStruct ",  id, " isDoublePV");

        if( entryExists(doublePVMap, id) )
        {
            pvs = &doublePVMap[id].pvs;
//            message("getPVStruct found ",  id, " in doublePVMap");
//            success = true;
        }
    }
    if( pvs )
    {
        debugMessage("getPVStruct found ", id);
    }
    else
    {
        message("getPVStruct failed to find ", id);
    }
    return pvs;
}
//______________________________________________________________________________
void VCgeneralMonitor::killMonitor(gmStructs::pvStruct& pvs)
{
    ca_clear_subscription( pvs.EVID );
    int status = sendToEpics(ca_clear_subscription_str,ca_csub_successmess,ca_csub_failuremess);
    if( status == ECA_TIMEOUT )
    {
        //removeChanel();
    }
    else if( status == ECA_NORMAL )
    {
        pvs.MonitorConnected = false;
        debugMessage(pvs.id, " killMonitor successfull.");
    }
    else
    {
         message(pvs.id, " killMonitor un-successfull.");
    }
}
//______________________________________________________________________________
void VCgeneralMonitor::killChannel(gmStructs::pvStruct& pvs)
{
    ca_clear_channel( pvs.CHID );
    int status = sendToEpics(ca_clear_channel_str,ca_cchi_successmess,ca_cchi_failuremess);
    if( status == ECA_TIMEOUT )
    {
        //removeChanel();
    }
    else if( status == ECA_NORMAL )
    {
        pvs.CHIDConnected = false;
        debugMessage(pvs.id, " killChannel successfull.");
    }
    else
    {
         message(pvs.id, " killChannel un-successfull.");
    }

}
//______________________________________________________________________________
template< class T >
bool VCgeneralMonitor::entryExists( std::map< std::string, T > & m, const std::string & name )
{
    bool ret = false;
    auto it = m.find( name );
        if( it != m.end() )
            ret = true;
    return ret;
}
//______________________________________________________________________________
template< class T >
bool VCgeneralMonitor::deleteMapEntry(std::map<std::string, T>& map, const std::string& id)
{
    bool r = false;
    std::map<std::string, T >::iterator it = map.find(id) ;
    if( it != map.end() )
    {
        map.erase( it );
        debugMessage(id," deleted");
        r = true;
    }
    else
        message("deleteMapEntry could not find id = ", id);
    return r;
}
//______________________________________________________________________________
bool VCgeneralMonitor::deleteID(const std::string& id )
{
    bool r = false;
    if(isIntPV(id))
    {
        r = deleteMapEntry(intPVMap,id);
    }
    else if(isFloatPV(id) )
    {
        r = deleteMapEntry(floatPVMap,id);
    }
    else if(isEnumPV(id) )
    {
        r = deleteMapEntry(enumPVMap,id);
    }
    else if(isCharPV(id) )
    {
        r = deleteMapEntry(charPVMap,id);
    }
    else if(isLongPV(id) )
    {
        r = deleteMapEntry(longPVMap,id);
    }
    else if(isDoublePV(id) )
    {
        r = deleteMapEntry(doublePVMap,id);
    }
    return r;
}
//______________________________________________________________________________
void VCgeneralMonitor::updateTime( const epicsTimeStamp & stamp, double & val, std::string & str )
{// look in epicsTime.h
//    const dbr_time_char * pTD = ( const struct dbr_time_char * ) args.dbr;
//    epicsUInt32 sec  = pTD ->stamp.secPastEpoch;   /* seconds since 0000 Jan 1, 1990 */
//    epicsUInt32 nsec = pTD ->stamp.nsec;
    char timeString[36];//MAGIC_NUMBER
    epicsTimeToStrftime ( timeString, sizeof ( timeString ), "%a %b %d %Y %H:%M:%S.%f", &stamp );
//    const dbr_char_t * pValue = &pTD -> value;
    val =  ( (double)stamp.nsec * 0.000000001 ) + (double)stamp.secPastEpoch;//MAGIC_NUMBER
    // prove it works
    //std::cout << std::setprecision(15) << std::showpoint<<   val << std::endl;
    str = timeString;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isStringPV(const std::string& id)
{
    return id.substr(0,1) == stringPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isIntPV(const std::string& id)
{
    return id.substr(0,1) == intPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isFloatPV(const std::string& id)
{
    return id.substr(0,1) == floatPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isEnumPV(const std::string& id)
{
    return id.substr(0,1) == enumPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isCharPV(const std::string& id)
{
    return id.substr(0,1) == charPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isLongPV(const std::string& id)
{
    return id.substr(0,1) == longPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isDoublePV(const std::string& id)
{
    return id.substr(0,1) == doublePrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isConnected(const std::string & id)
{
    gmStructs::pvStruct* pvs = getPVStruct_Ptr(id);
    if( pvs )
    {
        debugMessage("isConnected found ", id);
        return pvs->CHIDConnected;
    }
    else
    {
        message("isConnected did not find ", id);
        return false;
    }
}
//______________________________________________________________________________
bool VCgeneralMonitor::isMonitoring(const std::string & id)
{
    gmStructs::pvStruct* pvs = getPVStruct_Ptr(id);
    if(pvs)
    {
        debugMessage("isMonitoring found ", id);
        return pvs->MonitorConnected;
    }
    else
    {
        message("isMonitoring did not find ", id);
        return false;
    }
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToIntPVMap()
{
    std::string id = intPrefix;
    id += std::to_string(pvMonitorMapCount);
    intPVMap[id].id = id;
    intPVMap.at(id).pvs.id = id;
    intPVMap.at(id).pvs.CHTYPE = DBR_TIME_INT;
    debugMessage("connectPV Passed a DBR_INT. Entry with id =  ",id, " created");
    return &intPVMap[id].pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToFloatPVMap()
{
    std::string id = floatPrefix;
    id += std::to_string(pvMonitorMapCount);
    floatPVMap[id].id = id;
    floatPVMap.at(id).pvs.CHTYPE = DBR_TIME_FLOAT;
    floatPVMap.at(id).pvs.id = id;
    debugMessage("connectPV Passed a DBR_FLOAT. Entry with id =  ",id, " created");
    return &floatPVMap[id].pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToDoublePVMap()
{
    std::string id = doublePrefix;
    id += std::to_string(pvMonitorMapCount);
    doublePVMap[id].id = id;
    doublePVMap[id].pvs.CHTYPE = DBR_DOUBLE;
    doublePVMap[id].pvs.id = id;
    debugMessage("connectPV Passed a DBR_DOUBLE. Entry with id =  ",id, " created");
    return &doublePVMap[id].pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToCharPVMap()
{
    std::string id = charPrefix;
    id += std::to_string(pvMonitorMapCount);
    charPVMap[id].id = id;
    charPVMap.at(id).pvs.CHTYPE = DBR_TIME_CHAR;
    charPVMap.at(id).pvs.id = id;
    debugMessage("connectPV Passed a DBR_CHAR. Entry with id =  ",id, " created");
    return &charPVMap[id].pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToLongPVMap()
{
    std::string id = longPrefix;
    id += std::to_string(pvMonitorMapCount);
    longPVMap[id].id = id;
    longPVMap.at(id).pvs.CHTYPE = DBR_TIME_LONG;
    longPVMap.at(id).pvs.id = id;
    debugMessage("connectPV Passed a DBR_LONG. Entry with id =  ",id, " created");
    return &longPVMap[id].pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToEnumPVMap()
{
    std::string id = enumPrefix;
    id += std::to_string(pvMonitorMapCount);
    enumPVMap[id].id = id;
    enumPVMap.at(id).pvs.CHTYPE = DBR_TIME_ENUM;
    enumPVMap.at(id).pvs.id = id;
    debugMessage("connectPV Passed a DBR_ENUM. Entry with id =  ",id, " created");
    return &enumPVMap[id].pvs;
}
//______________________________________________________________________________
/// base class memebr functions
//______________________________________________________________________________
std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::ILOCK_STATE> VCgeneralMonitor::getILockStates(const std::string& name)
{
    std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::ILOCK_STATE> r;
    return r;
}
//______________________________________________________________________________
std::map<VELA_ENUM::ILOCK_NUMBER,std::string> VCgeneralMonitor::getILockStatesStr(const std::string & name)
{
    std::map<VELA_ENUM::ILOCK_NUMBER,std::string> r;
    return r;
}
//______________________________________________________________________________
double VCgeneralMonitor::get_CA_PEND_IO_TIMEOUT()
{
    return CA_PEND_IO_TIMEOUT;
}
//______________________________________________________________________________
void VCgeneralMonitor::set_CA_PEND_IO_TIMEOUT( double val )
{
    CA_PEND_IO_TIMEOUT = val;
}










