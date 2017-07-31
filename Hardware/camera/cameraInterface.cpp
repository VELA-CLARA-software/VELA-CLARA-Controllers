//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //

#include "cameraInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>

//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
cameraInterface::cameraInterface( //const std::string &Conf,
                                  //const bool startVirtualMachine,
                                  const bool* show_messages_ptr,
                                  const bool* show_debug_messages_ptr)://,
                                  //const bool shouldStartEPICs,
                                  //const VELA_ENUM::MACHINE_AREA myMachineArea ):
//configReader( Conf, startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
interface( show_messages_ptr, show_debug_messages_ptr ),
//shouldStartEPICs( shouldStartEPICs ), myMachineArea(myMachineArea),
selectedDAQCameraRef(selectedDAQCamera),
selectedIACamera(nullptr),
vcDAQCamera(nullptr),
vcIACamera(nullptr)
{
}
cameraInterface::~cameraInterface()
{
}

void cameraInterface::addChannel( const std::string & pvRoot, cameraStructs::pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1 );
}

bool cameraInterface::isON ( const std::string & cam )
{
    bool ans = false;
    if( entryExists( allCamDAQData, cam ) )
        if( allCamDAQData[cam].state == cameraStructs::CAM_STATE::CAM_ON )
            ans = true;
    return ans;
}
bool cameraInterface::isOFF( const std::string & cam )
{
    bool ans = false;
    if( entryExists( allCamDAQData, cam ) )
        if( allCamDAQData[cam].state == cameraStructs::CAM_STATE::CAM_OFF )
            ans = true;
    return ans;
}
bool cameraInterface::isAquiring( const std::string & cam )///WRITE
{
    bool ans = false;
    if( entryExists( allCamDAQData, cam ) )
        if( allCamDAQData[cam].state == cameraStructs::CAM_STATE::CAM_ACQUIRING )
            ans = true;
    return ans;
}
bool cameraInterface::isNotAquiring ( const std::string & cam)///WRITE
{
    bool ans = false;
    if( entryExists( allCamDAQData, cam ) )
        if( allCamDAQData[cam].state == cameraStructs::CAM_STATE::CAM_NOT_ACQUIRING )
            ans = true;
    return ans;
}
std::string cameraInterface::selectedCamera()
{
        return selectedDAQCamera.name;
}
bool cameraInterface::setCamera(const std::string & cam)
{
    bool ans = false;
    if( entryExists( allCamDAQData, cam ) )
    {
        //selectedDAQCamera = nullptr;
        selectedDAQCamera = allCamDAQData[cam];
        //selectedIACamera = allCamIAData[cam];
        ans = true;

        message("new setCamera = ",selectedDAQCamera.name);

    }
    return ans;
}
bool cameraInterface::startAquiring()///WRITE
{
    bool ans=false;
    unsigned short comm = 1;
    message("C++ selected camera aquiring:",isAquiring(selectedCamera()));
    if( isNotAquiring(selectedCamera()) )
    {
        ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHTYPE,
               selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHID,
               &comm);
               ans=true;
        message("sendToEpics ");
        int status = sendToEpics("ca_put", "", "Timeout trying to send new PSU states.");
        if(status == ECA_NORMAL)
        {
            ans = true;
            message("status return was ECA_NORMAL");
        }
        message("Sarting to Acquire images on ",selectedDAQCamera.name," camera.");
    }
    return ans;
}
bool cameraInterface::stopAquiring()///WRITE
{
    bool ans=false;
    unsigned short comm = 0;
    if( isAquiring(selectedCamera()) )
    {
        ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHTYPE,
               selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHID,
               &comm);
        message("sendToEpics ");
        int status = sendToEpics("ca_put", "", "Timeout trying to send new PSU states.");
        if(status == ECA_NORMAL)
        {
            ans = true;
            message("status return was ECA_NORMAL");
        }
        message("Stopping to Acquire images on ",selectedDAQCamera.name," camera.");
    }
    return ans;
}
bool cameraInterface::startVCAquiring()///WRITE
{
    return false;
}
bool cameraInterface::stopVCAquiring()///WRITE
{
    return false;
}

/*
}
void cameraInterface::updateX(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.x = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.x,value,allCamData[cameraName].imageStruct.xTolerance))
            debugMessage(cameraName," NEW X = ", allCamData[cameraName].rawData.x);
    }
}
void cameraInterface::updateY(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.y = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.y,value,allCamData[cameraName].imageStruct.yTolerance))
            debugMessage(cameraName," NEW Y = ", allCamData[cameraName].rawData.y);
    }
}
void cameraInterface::updateSigmaX(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.xSigma = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.xSigma,value,allCamData[cameraName].imageStruct.xSigmaTolerance))
            debugMessage(cameraName," NEW SIGMA X = ", allCamData[cameraName].rawData.xSigma);
    }
}
void cameraInterface::updateSigmaY(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.ySigma = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.ySigma,value,allCamData[cameraName].imageStruct.ySigmaTolerance))
            debugMessage(cameraName," NEW SIGMA Y = ", allCamData[cameraName].rawData.ySigma);
    }
}
void cameraInterface::updateSigmaXY(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.xySigma = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.xySigma,value,allCamData[cameraName].imageStruct.xySigmaTolerance))
            debugMessage(cameraName," NEW SIGMA XY = ", allCamData[cameraName].rawData.xySigma);
    }
}
bool cameraInterface::start()
{
    message("start() called");
    bool ret = false;
    unsigned short comm = (selectedCamera.state == cameraStructs::CAM_STATUS::CAM_ON) ? EPICS_SEND : EPICS_ACTIVATE;
    message("comm = ", comm);
        ca_put(selectedCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_START].CHTYPE,
               selectedCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_START].CHID,
               &comm);
        message("Trying to turn on camera named ", selectedCamera.name);
    int status = sendToEpics( "ca_put", "", "Timeout sending start value");
    if ( status == ECA_NORMAL )
        ret = true; message("status return was ECA_NORMAL");
    return ret;
}
bool cameraInterface::stop()
{
    message("start() called");
    bool ret = false;
    unsigned short comm = (selectedCamera.state == cameraStructs::CAM_STATUS::CAM_OFF) ? EPICS_SEND : EPICS_ACTIVATE;
    message("comm = ", comm);
        ca_put(selectedCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_START].CHTYPE,
               selectedCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_START].CHID,
               &comm);
        message("Trying to turn on camera named ", selectedCamera.name);
    int status = sendToEpics( "ca_put", "", "Timeout sending start value");
    if ( status == ECA_NORMAL )
        ret = true; message("status return was ECA_NORMAL");
    return ret;
}
*/
