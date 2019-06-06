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
//  Author:      TP, then DJS
//  Last edit:   13-05-2018
//  FileName:    cameraStructs.cpp
//  Description:
//
//
//
//
//*/
// project includes
#include "cameraStructs.h"
#include "cameraBase.h"
//
using namespace cameraStructs;
//
//             __           __   ___ ___ ___  ___  __   __
//  |\/|  /\  /__` |__/    /__` |__   |   |  |__  |__) /__`
//  |  | /~~\ .__/ |  \    .__/ |___  |   |  |___ |  \ .__/
//
//
//______________________________________________________________________________
void analysis_mask::maskX(int v)
{
    if(maskInterface)
        maskInterface->setMaskX(v);
}
//______________________________________________________________________________
void analysis_mask::maskY(int v)
{
    if(maskInterface)
        maskInterface->setMaskY(v);
}
//______________________________________________________________________________
void analysis_mask::maskXrad(int v)
{
    if(maskInterface)
        maskInterface->setMaskXrad(v);
}
//______________________________________________________________________________
void analysis_mask::maskYrad(int v)
{
    if(maskInterface)
        maskInterface->setMaskYrad(v);
}
//______________________________________________________________________________
//
//  __                         __      __  ___      ___  __            __   ___ ___ ___  ___  __   __
// |__) |  | |\ | |\ | | |\ | / _`    /__`  |   /\   |  /__`    __    / _` |__   |   |  |__  |__) /__`
// |  \ \__/ | \| | \| | | \| \__>    .__/  |  /~~\  |  .__/          \__> |___  |   |  |___ |  \ .__/
//
//
//
//
//
//  __  ___      ___  __            __
// /__`  |   /\   |  /__`    __    |__) | \_/
// .__/  |  /~~\  |  .__/          |    | / \
//
//
//______________________________________________________________________________
double analysis_data::x_pix_mean()
{
    return x_pix_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::y_pix_mean()
{
    return y_pix_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::sig_x_pix_mean()
{
    return sig_x_pix_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::sig_y_pix_mean()
{
    return sig_y_pix_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::sig_xy_pix_mean()
{
    return sig_xy_pix_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::x_pix_sd()
{
    return x_pix_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::y_pix_sd()
{
    return y_pix_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::sig_x_pix_sd()
{
    return sig_x_pix_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::sig_y_pix_sd()
{
    return sig_y_pix_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::sig_xy_pix_sd()
{
    return sig_xy_pix_rs.StandardDeviation();
}
//______________________________________________________________________________
//
//  __  ___      ___  __
// /__`  |   /\   |  /__`    __     |\/|  |\/|
// .__/  |  /~~\  |  .__/           |  |  |  |
//
//
//______________________________________________________________________________

double analysis_data::x_mean()
{
    return x_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::y_mean()
{
    return y_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::sig_x_mean()
{
    return sig_x_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::sig_y_mean()
{
    return sig_y_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::sig_xy_mean()
{
    return sig_xy_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::avg_pix_mean()
{
    return avg_pix_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::sum_pix_mean()
{
    return sum_pix_rs.Mean();
}
//______________________________________________________________________________
double analysis_data::x_sd()
{
    return x_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::y_sd()
{
    return y_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::sig_x_sd()
{
    return sig_x_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::sig_y_sd()
{
    return sig_y_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::sig_xy_sd()
{
    return sig_xy_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::avg_pix_sd()
{
    return avg_pix_rs.StandardDeviation();
}
//______________________________________________________________________________
double analysis_data::sum_pix_sd()
{
    return sum_pix_rs.StandardDeviation();
}
//______________________________________________________________________________
//
//
//  __        ___       __
// /  ` |    |__   /\  |__)
// \__, |___ |___ /~~\ |  \
//
//
//
//______________________________________________________________________________
void analysis_data::x_clear()
{
    x_rs.Clear();
}
//______________________________________________________________________________
void analysis_data::y_clear()
{
    y_rs.Clear();
}
//______________________________________________________________________________
void analysis_data::sig_x_clear()
{
    sig_x_rs.Clear();
}
//______________________________________________________________________________
void analysis_data::sig_y_clear()
{
    sig_y_rs.Clear();
}
//______________________________________________________________________________
void analysis_data::sig_xy_clear()
{
    sig_xy_rs.Clear();
}
//______________________________________________________________________________
void analysis_data::x_pix_clear()
{
    x_pix_rs.Clear();
}
//______________________________________________________________________________

void analysis_data::y_pix_clear()
{
    y_pix_rs.Clear();
}
//______________________________________________________________________________
void analysis_data::sig_x_pix_clear()
{
    sig_x_pix_rs.Clear();
}
//______________________________________________________________________________

void analysis_data::sig_y_pix_clear()
{
    sig_y_pix_rs.Clear();
}
//______________________________________________________________________________
void analysis_data::sig_xy_pix_clear()
{
    sig_xy_pix_rs.Clear();
}
//______________________________________________________________________________
void analysis_data::avg_pix_clear()
{
    avg_pix_rs.Clear();
}
//______________________________________________________________________________
void analysis_data::sum_pix_clear()
{
    sum_pix_rs.Clear();
}
//______________________________________________________________________________
//
//                    __       ___                              ___  __
//|\ | |  |  |\/|    |  \  /\   |   /\     \  /  /\  |    |  | |__  /__`
//| \| \__/  |  |    |__/ /~~\  |  /~~\     \/  /~~\ |___ \__/ |___ .__/
//
//
size_t analysis_data::x_n()
{
    return x_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::y_n()
{
    return y_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::sig_x_n()
{
    return sig_x_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::sig_y_n()
{
    return sig_y_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::sig_xy_n()
{
    return sig_xy_rs.NumDataValues();
}
//______________________________________________________________________________

size_t analysis_data::x_pix_n()
{
    return x_pix_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::y_pix_n()
{
    return y_pix_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::sig_x_pix_n()
{
    return sig_x_pix_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::sig_y_pix_n()
{
    return sig_y_pix_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::sig_xy_pix_n()
{
    return sig_xy_pix_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::avg_pix_n()
{
    return avg_pix_rs.NumDataValues();
}
//______________________________________________________________________________
size_t analysis_data::sum_pix_n()
{
    return sum_pix_rs.NumDataValues();
}







//______________________________________________________________________________
bool analysis_data::x_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::y_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::sig_x_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::sig_y_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::sig_xy_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::x_pix_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::y_pix_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::sig_x_pix_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::sig_y_pix_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::sig_xy_pix_full()
{
    return sig_xy_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::avg_pix_full()
{
    return sum_pix_rs.Full();
}
//______________________________________________________________________________
bool analysis_data::sum_pix_full()
{
    return sum_pix_rs.Full();
}

////-------------------------------------------------------------------------------
//void analysis_data::x_buffer_size(const size_t new_size)
//{
//    x_rs.NumDataValues(new_size);
//    x_lear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::y_buffer_size(const size_t new_size)
//{
//    y_rs.NumDataValues(new_size);
//    y_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::sig_x_buffer_size(const size_t new_size)
//{
//    sig_x_rs.NumDataValues(new_size);
//    sig_x_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::sig_y_buffer_size(const size_t new_size)
//{
//    sig_y_rs.NumDataValues(new_size);
//    sig_y_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::sig_xy_buffer_size(const size_t new_size)
//{
//    sig_xy_rs.NumDataValues(new_size);
//    sig_xy_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::x_pix_buffer_size(const size_t new_size)
//{
//    x_pix_rs.NumDataValues(new_size);
//    x_pix_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::y_pix_buffer_size(const size_t new_size)
//{
//    y_pix_rs.NumDataValues(new_size);
//    y_pix_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::sig_x_pix_buffer_size(const size_t new_size)
//{
//    sig_x_pix_rs.NumDataValues(new_size);
//    sig_x_pix_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::sig_y_pix_buffer_size(const size_t new_size)
//{
//    sig_y_pix_rs.NumDataValues(new_size);
//    sig_y_pix_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::sig_xy_pix_buffer_size(const size_t new_size)
//{
//    sig_xy_pix_rs.NumDataValues(new_size);
//    sig_xy_pix_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::avg_pix_buffer_size(const size_t new_size)
//{
//    avg_pix_rs.NumDataValues(new_size);
//    avg_pix_clear();
//}
////-------------------------------------------------------------------------------
//void analysis_data::sum_pix_buffer_size(const size_t new_size)
//{
//    sum_pix_rs.NumDataValues(new_size);
//    sum_pix_clear();
//}
////-------------------------------------------------------------------------------

//______________________________________________________________________________
#ifdef BUILD_DLL




//void fast_image::array_data_Py()
//{
//    if(fast_image_interface)
//    {
////        if(fast_image_interface->takeFastImage(name))
////        {
//            data = fast_image_interface->toPythonList<int>(array_data);
//            return data;
////        }
//    }
//    boost::python::list l(UTL::DUMMY_DOUBLE);
//    return l;
//}
////______________________________________________________________________________
//void fast_image::array_data2D_Py()
//{
//    if(fast_image_interface)
//    {
//        //if(fast_image_interface->takeFastImage(name))
//        //{
//            data2D = fast_image_interface->toPythonList2D<int>(array_data,num_pix_x,num_pix_y);
//        //}
//            return data2D;
//    }
//    boost::python::list l(UTL::DUMMY_DOUBLE);
//    return l;
//}
////______________________________________________________________________________
//boost::python::list fast_image::background_data_Py()
//{
//    if(fast_image_interface)
//        return fast_image_interface->toPythonList<int>(background_array);
//    else
//    {
//        boost::python::list l(UTL::DUMMY_DOUBLE);
//        return l;
//    }
//}
////______________________________________________________________________________
//boost::python::list fast_image::background_data_2D_Py()
//{
//    if(fast_image_interface)
//        return fast_image_interface->toPythonList2D<int>(background_array,
//                                                         num_pix_x,
//                                                         num_pix_y);
//    else
//    {
//        boost::python::list l(UTL::DUMMY_DOUBLE);
//        return l;
//    }
//}
#endif
