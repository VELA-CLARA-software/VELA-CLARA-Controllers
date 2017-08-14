#include "cameraImageCropper.h"
// djs
#include "globalVelaCams.h"
// stl
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <functional>
#include <numeric>

cameraImageCropper::cameraImageCropper(  )
{

    icsData = globalVelaCams::getImgCutParamMap();

    std::cout << "x0 = " << icsData["YAG06"].x0 << std::endl;
    std::cout << "y0 = " << icsData["YAG06"].y0 << std::endl;
    std::cout << "xRad = " << icsData["YAG06"].xRad << std::endl;
    std::cout << "yRad = " << icsData["YAG06"].yRad << std::endl;
    std::cout << "xstart = " << icsData["YAG06"].xstart << std::endl;
    std::cout << "xstop = " << icsData["YAG06"].xstop << std::endl;
    std::cout << "ystart = " << icsData["YAG06"].ystart << std::endl;
    std::cout << "ystop = " << icsData["YAG06"].ystop << std::endl;
    std::cout << "numPixX = " << icsData["YAG06"].numPixX << std::endl;
    std::cout << "numPixY = " << icsData["YAG06"].numPixY << std::endl;
    std::cout << "totNumPix = " << icsData["YAG06"].totNumPix << std::endl;
    std::cout << "numX = " << icsData["YAG06"].numX << std::endl;
    std::cout << "numY = " << icsData["YAG06"].numY << std::endl;
    std::cout << "xPix2mm = " << icsData["YAG06"].xPix2mm << std::endl;
    std::cout << "yPix2mm = " << icsData["YAG06"].yPix2mm << std::endl;


}
//______________________________________________________________________________
cameraImageCropper::~cameraImageCropper()
{
}

velaCamStructs::imgDatC cameraImageCropper::cropImageToScreen( const std::vector< unsigned char > & data, std::string & screen )
{
    velaCamStructs::imgDatC c;


    /// globalVelaCams::getImgCutParamMap() fills icsData, this knows ellipse centre and radii
    /// it crops the data to +- 1 radius in x nad y, of course limited by the max number of pixels in x and y
    /// later when you apply the mask it does not matter if x and y rad are larger than the dimensions of the image

    c.data.reserve( icsData[ screen ].numX * icsData[ screen ].numY );

    for( int y = 0; y <  icsData[ screen ].numY ; ++y )
    {
        for( int x = 0; x <  icsData[ screen ].numX; ++x )
        {
            c.data.push_back( data[ ( icsData[ screen ].ystart + y ) * icsData[ screen ].numPixX  + icsData[ screen ].xstart + x ] );
        }
    }

    c.numX = icsData[ screen ].numX;
    c.numY = icsData[ screen ].numY;

//    double sum_of_elems = std::accumulate(vecToFil.begin(),vecToFil.end(),0);
//
//    std::cout << "Num X elements = " << icsData[ screen ].numX << std::endl;
//    std::cout << "Num Y elements = " << icsData[ screen ].numY << std::endl;
//    std::cout << "xstart = " << icsData[ screen ].xstart << std::endl;
//    std::cout << "xstop = " << icsData[ screen ].xstop << std::endl;
//    std::cout << "ystart = " << icsData[ screen ].ystart << std::endl;
//    std::cout << "ystop = " << icsData[ screen ].ystop << std::endl;
//    std::cout << "vecToFil.size() = " << vecToFil.size()  << std::endl;
//    std::cout << "Hash After Cropping = " << sum_of_elems << std::endl;

    return c;
}
//______________________________________________________________________________
velaCamStructs::imgDatC cameraImageCropper::cropImage( const std::vector< unsigned char > & data, velaCamStructs::imageCutStruct & icsData )
{
    ///sanity check parameters
    sanityCheckImageCutStuct( icsData );
    velaCamStructs::imgDatC ret;

    ret.data.resize( icsData.numY * icsData.numX );
    ret.numX = icsData.numX;
    ret.numY = icsData.numY;

    for( size_t y = 0; y <  icsData.numY ; ++y )
        for( size_t x = 0; x <  icsData.numX; ++x )
            ret.data[ x + y *  icsData.numX ] = data[ ( icsData.ystart + y ) * icsData.numPixX  + icsData.xstart + x ];

    return ret;
}
//______________________________________________________________________________
velaCamStructs::imgDatD cameraImageCropper::cropImage(const std::vector< double > & data, velaCamStructs::imageCutStruct & icsData )
{
    ///sanity check parameters
    sanityCheckImageCutStuct( icsData );

    velaCamStructs::imgDatD ret;

    //ret.data.resize( icsData.numY * icsData.numX );
    ret.data.reserve( icsData.numY * icsData.numX );
    ret.numX = icsData.numX;
    ret.numY = icsData.numY;

    for( size_t y = 0; y <  icsData.numY ; ++y )
        for( size_t x = 0; x <  icsData.numX; ++x )
            ret.data.push_back( data[ ( icsData.ystart + y ) * icsData.numPixX  + icsData.xstart + x ] );
            //ret.data[ x + y *  icsData.numX ] = data[ ( icsData.ystart + y ) * icsData.numPixX  + icsData.xstart + x ];

    return ret;
}

void cameraImageCropper::sanityCheckImageCutStuct( velaCamStructs::imageCutStruct & icsData )
{
    /// assum indexes go from 0 upwards


    icsData.xstart =  icsData.x0  -  icsData.xRad;
    icsData.xstop  =  icsData.x0  +  icsData.xRad;
    icsData.ystart =  icsData.y0  -  icsData.yRad;
    icsData.ystop  =  icsData.y0  +  icsData.yRad;

    /// SANITY Checks

    if( icsData.xstop >  icsData.numPixX - 1 )
        icsData.xstop =  icsData.numPixX - 1;
    if( icsData.xstart < 0 )
        icsData.xstart = 0;

    if( icsData.ystop >  icsData.numPixY - 1 )
        icsData.ystop =  icsData.numPixY - 1;
    if( icsData.ystart < 0 )
        icsData.ystart = 0;

    /// The plus one is because we want to include from xstart to xstop and ystart and ystop

    icsData.numX = icsData.xstop - icsData.xstart + 1;
    icsData.numY = icsData.ystop - icsData.ystart + 1;
}



//______________________________________________________________________________
std::vector< double > cameraImageCropper::getYProj( velaCamStructs::imgDatC & idc )
{
    std::vector< double > vec( idc.numY );
    for( size_t y = 0; y < idc.numY; ++y )
    {
        for( size_t x = 0; x < idc.numX; ++x )
        {
            vec[y] += (double)idc.data[ y * idc.numX + x ];
        }
    }
    return vec;
}
//______________________________________________________________________________
std::vector< double > cameraImageCropper::getYProj( velaCamStructs::imgDatD & idc )
{
    std::vector< double > vec( idc.numY );
    for( size_t y = 0; y < idc.numY; ++y )
    {
        for( size_t x = 0; x < idc.numX; ++x )
        {
            vec[y] += idc.data[ y * idc.numX + x ];
        }
    }
    return vec;
}
//______________________________________________________________________________
std::vector< double > cameraImageCropper::getXProj( velaCamStructs::imgDatC & idc )
{
    std::vector< double > vec( idc.numX );
    for( size_t x = 0; x < idc.numX; ++x )
    {
        for( size_t y = 0; y < idc.numY; ++y )
        {
            vec[ x ] += (double)idc.data[ y * idc.numX + x ];
        }
    }
    return vec;
}
//______________________________________________________________________________
std::vector< double > cameraImageCropper::getXProj( velaCamStructs::imgDatD & idc )
{
    std::vector< double > vec( idc.numX );
    for( size_t x = 0; x < idc.numX; ++x )
    {
        for( size_t y = 0; y < idc.numY; ++y )
        {
            vec[ x ] += idc.data[ y * idc.numX + x ];
        }
    }
    return vec;
}
















/// MASKS N STUFF
/// MASKS N STUFF
/// MASKS N STUFF
/// MASKS N STUFF
/// MASKS N STUFF
/// MASKS N STUFF
/// MASKS N STUFF




void cameraImageCropper::applyMask(std::vector< unsigned char > & data, std::string & screen )
{
    /// This assumes the data is already cropped ??


    if( !maskData.count(screen) ) // If this exists.
    {
        std::cout << "Mask Doesn't exist for " << screen << ", creating mask..." << std::endl;
        calcMask( screen );
        std::cout << "Mask Created!" << std::endl;
    }
    if(  maskData[ screen ].size() != data.size() )
    {

        std::cout << std::endl;
        std::cout << "MAJOR ERROR The cameraImageCropper can't apply mask, vectors of different size" << std::endl;
        std::cout << std::endl;
        std::cout << "maskData[ screen ].size() = " << maskData[ screen ].size() << " and data.size() = " << data.size() << std::endl;
        std::cout << std::endl;
    }
    else{

        //std::cout << "*** APPLYING MASK ***";
        std::transform(  data.begin(), data.end(), maskData[ screen ].begin(), data.begin(), std::multiplies<unsigned char >() );
        //std::cout << " FIN " << std::endl;

        double sum_of_elems = std::accumulate(data.begin(),data.end(),0);

        //std::cout << "Num X elements = " << icsData[ screen ].numX << std::endl;
        //std::cout << "Num Y elements = " << icsData[ screen ].numY << std::endl;
        //std::cout << "Hash After Mask = " << sum_of_elems << std::endl;
    }
}


std::vector< unsigned char > cameraImageCropper::getMask( std::string & screen )
{
    if( !maskData.count(screen) ) // If this exists.
    {
         calcMask( screen );
    }
     return maskData[ screen ];
}

void cameraImageCropper::calcMask( std::string & screen)
{

    /// The mask is an ellipse that is bounded by the cropped rectangular image,
    /// all elements outside the elipse are zero, all inidie are 1.0

    if( !maskData.count(screen) ) // If the mask does not exist...
    {
        std::vector< unsigned char > mask;
        mask.clear();
        mask.resize(  icsData[ screen ].numX *  icsData[ screen ].numY );

        /// the exact centre of the mask is { h, k }

        double h, k;

        h = (double)  icsData[ screen ].numX / 2.0;
        k = (double)  icsData[ screen ].numY / 2.0;

//        if(  icsData[ screen ].numX % 2 == 0  )
//            h = (double)  icsData[ screen ].numX / 2.0;        // even
//        else
//            h = (double)  icsData[ screen ].numX / 2.0 - 0.5;  // odd
//
//        if(  icsData[ screen ].numY % 2 == 0  )
//            k = (double)  icsData[ screen ].numY / 2.0;        // even
//        else
//            k = (double)  icsData[ screen ].numY / 2.0 - 0.5;  // odd

        std::cout << "mask h = " << h << ", mask k = " << k << std::endl;
        std::cout << "Finding Pixels in ellipse ";

        /// so here we are using the lower left hand corner of a pixel, i guess it' would be better to use the coordinate of the pixel centre...
        /// meh..

        for( size_t j = 0; j <  icsData[ screen ].numY; ++j )
        {
            for( size_t i = 0; i <  icsData[ screen ].numX; ++i )
            {
                if( inEllipse(  icsData[ screen ], i , j , h, k) )
                    mask[ j *  icsData[ screen ].numX + i ] = 1;
                else
                    mask[ j *  icsData[ screen ].numX + i ] = 0;
            }
        }
        std::cout << "...complete" << std::endl;
        maskData[ screen  ] = mask;
        double sum_of_elems = std::accumulate(mask.begin(), mask.end(), 0);
        std::cout << "Hash mask = " << sum_of_elems << std::endl;
    }
}
//______________________________________________________________________________
bool cameraImageCropper::inEllipse( velaCamStructs::imgCutStruct & ics, size_t xCoord, size_t yCoord, double h, double k)
{
    /// h and k are real as they are the position, in pixels,
    /// of the centre of the pixel
    /// This function determines if {h, k} is within the ellipse (already centred at { x,0, y0 }

    if( fabs( (double) xCoord - h ) > ics.xRad   )
        return false;
    if( fabs( (double) yCoord - k ) > ics.yRad   )
        return false;
    if(  ( pow( ( (double) xCoord - h ) / (double)  ics.xRad, 2.0 ) + pow( ( (double)  yCoord - k ) / (double) ics.yRad, 2.0 ) ) > 1 )
        return false;
    else
        return true;
}
//______________________________________________________________________________
//void cameraImageCropper::checkICSData( std::string & screen )
//{
//    if( !icsData.count(screen) )
//         icsData[ screen ] = globalVelaCams::getImgCutParam( screen );
//}


