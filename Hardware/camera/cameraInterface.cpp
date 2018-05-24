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

//tp
#include "cameraInterface.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
using namespace cameraStructs;

cameraInterface::cameraInterface(bool& show_messages_ptr,
                                 bool& show_debug_messages_ptr):
interface(show_messages_ptr, show_debug_messages_ptr),
selectedCameraRef(selectedCameraObj),
vcCameraRef(vcCameraObj)

{
}
cameraInterface::~cameraInterface()
{
}
void cameraInterface::addChannel( const std::string & pvRoot, pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1 );
}
///Functions Accessible to Python Controller///
bool cameraInterface::isON ( const std::string & cam )
{
    std::string cameraName = useCameraFrom(cam);
    bool ans = false;
    if(entryExists( allCamData, cameraName))
        if(allCamData.at(cameraName).state == CAM_STATE::CAM_ON)
            ans = true;
    return ans;
}
bool cameraInterface::isOFF( const std::string & cam )
{
    std::string cameraName = useCameraFrom(cam);
    bool ans = false;
    if( entryExists( allCamData, cameraName ) )
        if(allCamData.at(cameraName).state == CAM_STATE::CAM_OFF)
            ans = true;
    return ans;
}
bool cameraInterface::isAcquiring( const std::string & cam )
{
    std::string cameraName = useCameraFrom(cam);
    bool ans = false;
    if( entryExists( allCamData, cameraName ) )
        if( allCamData.at(cameraName).acquireState == ACQUIRE_STATE::ACQUIRING )
            ans = true;
    return ans;
}
bool cameraInterface::isNotAcquiring ( const std::string & cam)
{
    std::string cameraName = useCameraFrom(cam);
    bool ans = false;
    if( entryExists( allCamData, cameraName ) )
        if( allCamData.at(cameraName).acquireState == ACQUIRE_STATE::NOT_ACQUIRING )
            ans = true;
    return ans;
}
std::string cameraInterface::selectedCamera()
{
        return selectedCameraObj.name;
}
bool cameraInterface::setCamera(const std::string & cam)
{
    std::string cameraName = useCameraFrom(cam);
    bool ans = false;
    if( entryExists( allCamData, cameraName ) )
    {
        selectedCameraObj = allCamData.at(cameraName);
        vcCameraObj = allCamData.at("VC");// MAGIC_STRING
        ans = true;
        message("new setCamera = ",selectedCameraObj.name);

    }
    return ans;
}
cameraInterface::vec_s cameraInterface::getCameraNames()
{
    vec_s r;
    for(auto && it:allCamData)
        r.push_back(it.first);
    //r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
/// this could be neatend up - much code repetition
bool cameraInterface::startAcquiring()
{
    bool ans=false;
    unsigned short comm = 1;
    if( isNotAcquiring(selectedCamera()))
    {
        pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::CAM_START_ACQUIRE));
        ans=shortCaput(comm,S);
        message("Starting to Acquire images on ",
                selectedCameraObj.name," camera.");
    }
    return ans;
}
bool cameraInterface::stopAcquiring()
{
    bool ans=false;
    unsigned short comm = 0;
    if( isAcquiring(selectedCamera()) && isCollecting(selectedCamera())==false)
    {
        pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::CAM_STOP_ACQUIRE));
        ans=shortCaput(comm,S);
        message("Stopping to Acquire images on ",
                selectedCameraObj.name," camera.");
    }
    return ans;
}
bool cameraInterface::startVCAcquiring()
{
    bool ans=false;
    unsigned short comm = 1;
    if( isNotAcquiring("VC") )
    {
        pvStruct S(vcCameraObj.pvComStructs.at(CAM_PV_TYPE::CAM_START_ACQUIRE));
        ans=shortCaput(comm,S);
        message("Starting to Acquire images on ",vcCameraObj.name," camera.");
    }
    return ans;
}
bool cameraInterface::stopVCAcquiring()
{
    bool ans=false;
    unsigned short comm = 0;
    if( isAcquiring("VC") && isCollecting("VC")==false )
    {
        pvStruct S(vcCameraObj.pvComStructs.at(CAM_PV_TYPE::CAM_STOP_ACQUIRE));
        ans=shortCaput(comm,S);
        message("Stopping to Acquire images on ",vcCameraObj.name," camera.");
    }
    return ans;
}
///Useful Functions for the Controller///
bool cameraInterface::isCollecting(const std::string&cameraName)
{
    bool ans = false;

    if (allCamData.at(cameraName).DAQ.captureState==1)
        ans=true;
    else if (allCamData.at(cameraName).DAQ.captureState==0)
        ans=false;
    else
        debugMessage("Problem with isCollecting() function.");
    return ans;
}
bool cameraInterface::isSaving(const std::string&cameraName)
{
    bool ans = false;
    if (allCamData.at(cameraName).DAQ.writeState==1)
        ans=true;
    else if (allCamData.at(cameraName).DAQ.writeState==0)
        ans=false;
    else
        debugMessage("Problem with isSaving() function.");
    return ans;
}

bool cameraInterface::isCollectingJPG(const std::string&cameraName)
{
    bool ans = false;

    if (allCamData.at(cameraName).DAQ.captureStateJPG==1)
        ans=true;
    else if (allCamData.at(cameraName).DAQ.captureStateJPG==0)
        ans=false;
    else
        debugMessage("Problem with isCollectingJPG() function.");
    return ans;
}
bool cameraInterface::isSavingJPG(const std::string&cameraName)
{
    bool ans = false;
    if (allCamData.at(cameraName).DAQ.writeStateJPG==1)
        ans=true;
    else if (allCamData.at(cameraName).DAQ.writeStateJPG==0)
        ans=false;
    else
        debugMessage("Problem with isSavingJPG() function.");
    return ans;
}
std::string cameraInterface::useCameraFrom(const std::string camOrScreen)
{
    std::string cameraName;
    if (entryExists(allCamData,camOrScreen))
        cameraName=camOrScreen;
    else
    {
        bool usingAScreenName=false;
        for (auto && it : allCamData)
        {
            if (it.second.screenName==camOrScreen)
            {
                cameraName=it.second.name;
                usingAScreenName=true;
            }
        }
        if (usingAScreenName==false)
        {
            message("ERROR: Controller does not recognise the name used.");
            cameraName="UNKNOWN";
        }
    }
    return cameraName;

 }
bool cameraInterface::shortCaput(const unsigned short &comm, pvStruct& S)
{
    bool ans(false);
    ca_put(S.CHTYPE,S.CHID, &comm);
    int status = sendToEpics("ca_put", "", "Timeout trying to sendToEpics.");
    if(status == ECA_NORMAL)
    {
        ans = true;
    }
    return ans;
}

bool cameraInterface::doubleCaput(const double &comm, pvStruct& S)
{
    bool ans(false);
    ca_put(S.CHTYPE,S.CHID, &comm);
    int status = sendToEpics("ca_put", "", "Timeout trying to sendToEpics.");
    if(status == ECA_NORMAL)
    {
        ans = true;
    }
    return ans;
}
