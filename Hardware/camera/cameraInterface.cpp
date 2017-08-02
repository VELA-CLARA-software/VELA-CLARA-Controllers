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

cameraInterface::cameraInterface(const bool* show_messages_ptr,
                                 const bool* show_debug_messages_ptr):
interface(show_messages_ptr, show_debug_messages_ptr), selectedDAQCameraRef(selectedDAQCamera),vcDAQCameraRef(vcDAQCamera),
selectedIACamera(nullptr),vcIACamera(nullptr)
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
bool cameraInterface::isAquiring( const std::string & cam )
{
    bool ans = false;
    if( entryExists( allCamDAQData, cam ) )
        if( allCamDAQData[cam].aquireState == cameraStructs::AQUIRE_STATE::ACQUIRING )
            ans = true;
    return ans;
}
bool cameraInterface::isNotAquiring ( const std::string & cam)
{
    bool ans = false;
    if( entryExists( allCamDAQData, cam ) )
        if( allCamDAQData[cam].aquireState == cameraStructs::AQUIRE_STATE::NOT_ACQUIRING )
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
        selectedDAQCamera = allCamDAQData[cam];
        vcDAQCamera = allCamDAQData["VC"];
        //selectedIACamera = allCamIAData[cam];
        ans = true;

        message("new setCamera = ",selectedDAQCamera.name);

    }
    return ans;
}
bool cameraInterface::startAquiring()
{
    bool ans=false;
    unsigned short comm = 1;
    message("C++ selected camera aquiring:",isAquiring(selectedCamera()));
    if( isNotAquiring(selectedCamera()) )
    {
        ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHTYPE,
               selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHID,
               &comm);
        int status = sendToEpics("ca_put", "", "Timeout trying to send new Aquiring state.");
        if(status == ECA_NORMAL)
            ans = true;
        message("Starting to Acquire images on ",selectedDAQCamera.name," camera.");
    }
    return ans;
}
bool cameraInterface::stopAquiring()
{
    bool ans=false;
    unsigned short comm = 0;
    if( isAquiring(selectedCamera()) )
    {
        ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHTYPE,
               selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHID,
               &comm);
        int status = sendToEpics("ca_put", "", "Timeout trying to send new Aquiring state.");
        if(status == ECA_NORMAL)
            ans = true;
        message("Stopping to Acquire images on ",selectedDAQCamera.name," camera.");
    }
    return ans;
}
bool cameraInterface::startVCAquiring()
{
    bool ans=false;
    unsigned short comm = 1;
    if( isNotAquiring("VC") )
    {  ;
        ca_put(vcDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHTYPE,
               vcDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHID,
               &comm);
        int status = sendToEpics("ca_put", "", "Timeout trying to send new Aquiring state.");
        if(status == ECA_NORMAL)
            ans = true;
        message("Starting to Acquire images on ",vcDAQCamera.name," camera.");
    }
    return ans;
}
bool cameraInterface::stopVCAquiring()
{
    bool ans=false;
    unsigned short comm = 0;
    if( isAquiring("VC") )
    {
        ca_put(vcDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHTYPE,
               vcDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE].CHID,
               &comm);
        int status = sendToEpics("ca_put", "", "Timeout trying to send new Aquiring state.");
        if(status == ECA_NORMAL)
            ans = true;
        message("Stopping to Acquire images on ",vcDAQCamera.name," camera.");
    }
    return ans;
}
