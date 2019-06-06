/*
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
//
//  Author:      DJS
//  Last edit:   28-06-2018
//  FileName:    pilaserStructs.cpp
//  Description:
//
//
//*/

#include "pilaserStructs.h"
//#include "pilaserInterface.h"
//______________________________________________________________________________
//
//  __                         __      __  ___      ___  __            __   ___ ___ ___  ___  __   __
// |__) |  | |\ | |\ | | |\ | / _`    /__`  |   /\   |  /__`    __    / _` |__   |   |  |__  |__) /__`
// |  \ \__/ | \| | \| | | \| \__>    .__/  |  /~~\  |  .__/          \__> |___  |   |  |___ |  \ .__/
//
//______________________________________________________________________________

void  pilaserStructs::pilaserObject::Q_clear()
{
    Q_rs.Clear();
}
//______________________________________________________________________________
void  pilaserStructs::pilaserObject::energy_clear()
{
    energy_rs.Clear();
}
//______________________________________________________________________________
double pilaserStructs::pilaserObject::Q_mean()
{
    return Q_rs.Mean();
}
//______________________________________________________________________________
double pilaserStructs::pilaserObject::energy_mean()
{
    return energy_rs.Mean();
}
//______________________________________________________________________________
double pilaserStructs::pilaserObject::Q_sd()
{
    return Q_rs.StandardDeviation();
}
//______________________________________________________________________________
double pilaserStructs::pilaserObject::energy_sd()
{
    return energy_rs.StandardDeviation();
}
//______________________________________________________________________________
size_t pilaserStructs::pilaserObject::Q_n()
{
    return Q_rs.NumDataValues();
}
//______________________________________________________________________________
size_t pilaserStructs::pilaserObject::energy_n()
{
    return energy_rs.NumDataValues();
}
//______________________________________________________________________________
void pilaserStructs::pilaserObject::Q_rs_buffer_size(size_t new_size)
{
     Q_rs.setMaxCount(new_size);
     Q_clear();
}
//______________________________________________________________________________
void pilaserStructs::pilaserObject::energy_rs_buffer_size(size_t new_size)
{
     energy_rs.setMaxCount(new_size);
     energy_clear();
}
//______________________________________________________________________________
bool pilaserStructs::pilaserObject::Q_full()
{
     return Q_rs.Full();
}
//______________________________________________________________________________
bool pilaserStructs::pilaserObject::energy_full()
{
     return energy_rs.Full();
}
