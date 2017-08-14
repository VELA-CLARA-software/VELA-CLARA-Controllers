#include "cameraDataObject.h"
// djs
#include "globalVelaCams.h"
// stl
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <functional>

cameraDataObject::cameraDataObject( std::string & name ) : dataIsSet( false ), backIsSet( false ),
backIsCut( false ), maskSet( false ), cutParamSet ( false )
{
    /// dataStruct is a camDataStruct that holds all Offline info about this camera image

    dataStruct.camName = name;

    /// set dummy dataStruct.imgParam
    /// imgParam is an imgCutStruct, it's how we know where to cut the image to get the screen (or user defined)

    dataStruct.imgParam = globalVelaCams::getImgCutParam( name );
}
//______________________________________________________________________________
cameraDataObject::~cameraDataObject()
{
}
//______________________________________________________________________________
/// _______                                  ______         __        ______
///|_     _|.--------.---.-.-----.-----.    |      |.--.--.|  |_     |   __ \.---.-.----.---.-.--------.
/// _|   |_ |        |  _  |  _  |  -__|    |   ---||  |  ||   _|    |    __/|  _  |   _|  _  |        |
///|_______||__|__|__|___._|___  |_____|    |______||_____||____|    |___|   |___._|__| |___._|__|__|__|
///                        |_____|
///
/// imgParam is an imgCutStruct that defines how the full image is cut to get just the screen
///
//______________________________________________________________________________
velaCamStructs::imgCutStruct cameraDataObject::getIMGCutParam()
{
    return dataStruct.imgParam;
}
//______________________________________________________________________________
void cameraDataObject::setIMGCutParam( velaCamStructs::imgCutStruct & p )
{
    dataStruct.imgParam = p;
    dataStruct.imgParam.xstart = p.x0 - p.xRad;
    dataStruct.imgParam.xstop  = p.x0 + p.xRad;
    dataStruct.imgParam.ystart = p.y0 - p.yRad;
    dataStruct.imgParam.ystop  = p.y0 + p.yRad;
    dataStruct.imgParam.numY   = dataStruct.imgParam.ystop - dataStruct.imgParam.ystart;
    dataStruct.imgParam.numX   = dataStruct.imgParam.xstop - dataStruct.imgParam.xstart;
    cutParamSet = true;
}
//______________________________________________________________________________
size_t cameraDataObject::getNumPix()
{
    return dataStruct.imgParam.totNumPix;
}
//______________________________________________________________________________
/// _______         __        _____         __              ______         ___
///|     __|.-----.|  |_     |     \.---.-.|  |_.---.-.    |   __ \.-----.'  _|.-----.
///|    |  ||  -__||   _|    |  --  |  _  ||   _|  _  |    |      <|  -__|   _||__ --|
///|_______||_____||____|    |_____/|___._||____|___._|    |___|__||_____|__|  |_____|
///
/// Raw and Proc(essed) data are held in two vectors, these get references to these vectors,
/// or to a part an integer number of totalNumPixels in, so we can add data,
/// I could write some resize functions here, to allow more data to be added than initially expected
///
//______________________________________________________________________________
std::vector< unsigned char > & cameraDataObject::getRawRef( velaCamStructs::DATA_TYPE type )
{
    switch( type )
    {
        case velaCamStructs::k_DATA:
            return data;
        case velaCamStructs::k_BACK:
            return back;
        default:
            return data;
    }
}
//______________________________________________________________________________
unsigned char * cameraDataObject::getRawRef( velaCamStructs::DATA_TYPE type, size_t part )
{
    unsigned char * ret = NULL;
    switch( type )
    {
        case velaCamStructs::k_DATA:
            if( part == 0 )
                ret = &data[ 0 ];
            else
                ret = &data[ part * dataStruct.imgParam.totNumPix - 1 ];
            break;
        case velaCamStructs::k_BACK:
            if( part == 0 )
                ret = &back[ 0 ];
            else
                ret = &back[ part * dataStruct.imgParam.totNumPix - 1 ];
            break;
    }
    return ret;
}
//______________________________________________________________________________
std::vector< double > & cameraDataObject::getProcRef( velaCamStructs::DATA_TYPE type )
{
    switch( type )
    {
        case velaCamStructs::k_DATA:
            return procData;
        case velaCamStructs::k_BACK:
            return procBack;
        default:
            return procData;
    }
}
//______________________________________________________________________________
double * cameraDataObject::getProcRef( velaCamStructs::DATA_TYPE type, size_t part )
{
    double * ret = NULL;
    switch( type )
    {
        case velaCamStructs::k_DATA:
            if( part == 0 )
                ret = &procData[ 0 ];
            else
                ret = &procData[ part * dataStruct.imgParam.totNumPix - 1 ];
            break;
        case velaCamStructs::k_BACK:
            if( part == 0 )
                ret = &procBack[ 0 ];
            else
                ret = &procBack[ part * dataStruct.imgParam.totNumPix - 1 ];
            break;
        default:
            break;
    }
    return ret;
}
//______________________________________________________________________________
void cameraDataObject::resetRaw( velaCamStructs::DATA_TYPE type, size_t numShots )
{
    dataStruct.imgParam.totNumPix = 1447680;

    switch( type )
    {
        case velaCamStructs::k_DATA:
            data.clear();
            data.resize( numShots * dataStruct.imgParam.totNumPix );
            dataPos = 0;
            setDataIsSet( false );
//            std::cout << "resetRaw numShots = " << numShots << std::endl;
//            std::cout << "resetRaw dataStruct.imgParam.totNumPix = " << dataStruct.imgParam.totNumPix << std::endl;
//            std::cout << "resetRaw new data.size() = " << data.size() << std::endl;
            break;
        case velaCamStructs::k_BACK:
            back.clear();
            back.resize( numShots * dataStruct.imgParam.totNumPix );
            backPos = 0;
            setBackIsSet( false );
//            std::cout << "resetRaw numShots = " << numShots << std::endl;
//            std::cout << "resetRaw dataStruct.imgParam.totNumPix = " << dataStruct.imgParam.totNumPix << std::endl;
//            std::cout << "resetRaw new back.size() = " << back.size() << std::endl;
            break;
    }
}
//______________________________________________________________________________
void cameraDataObject::setDataAsBack()
{
    back.clear();
    back = data;
    setBackIsSet( true );
}

/// ______                                         _____         __
///|   __ \.----.-----.----.-----.-----.-----.    |     \.---.-.|  |_.---.-.
///|    __/|   _|  _  |  __|  -__|__ --|__ --|    |  --  |  _  ||   _|  _  |
///|___|   |__| |_____|____|_____|_____|_____|    |_____/|___._||____|___._|
///
/// Cut the data up, apply masks ,etc, NOT YET FINISHED

    /// I need ot think about applyign the masks but keeping things as an unsingned char array to maintin speed
    /// it is probably going to make a big difference... ?
    /// should probably do some tests with mathematica

//______________________________________________________________________________
bool cameraDataObject::process( velaCamStructs::DATA_TYPE type )
{
   // std::cout << "cameraDataObject::preProcessData()"  << std::endl;
    bool success = false;
    bool carryon = true;

    switch( type )
    {
        case velaCamStructs::k_DATA:
            if( DataIsSet() )
            {
                if( DataIsNotCut() )
                    carryon = cut( type );
                std::cout << "cameraDataObject::processData cut( k_DATA ) = " << carryon  << std::endl;
                if( carryon )
                {
                    carryon = applyMask( type );
                    std::cout << "cameraDataObject::processData applyMask( k_DATA ) = " << carryon  << std::endl;
                    success = carryon;
                }
            }
            else
            {
                std::cout << "process Data Failed Because DataIsSet() = false "  << std::endl;
            }
            break;
        case velaCamStructs::k_BACK:
            if( BackIsSet() )
            {
                if( BackIsNotCut() )
                    carryon = cut( type );
                std::cout << "cameraDataObject::processData cut( k_DATA ) = " << carryon  << std::endl;
                if( carryon )
                {
                    carryon = applyMask( type );
                    std::cout << "cameraDataObject::processData applyMask( k_DATA ) = " << carryon  << std::endl;
                    success = carryon;
                }
            }
            else
            {
                std::cout << "process Data Failed Because DataIsSet() = false "  << std::endl;
            }
    }


    return success;
}
//______________________________________________________________________________

    /// The following could probably be re-written by passing in a pointer / reference to the vector
    /// and then cutting the passed array???
    /// more re-factoring

bool cameraDataObject::cut( velaCamStructs::DATA_TYPE type )
{
    bool ret = false;
    if( positiveCutValues() )
    {
        switch( type )
        {
            case velaCamStructs::k_DATA:
                if( DataIsSet() )
                {
                 //   std::cout<< "cut k_DATA ";
                    procData.clear();
                    procData.resize( dataStruct.imgParam.numX * dataStruct.imgParam.numY ); /// set in positiveCutValues();

    /// WE SHOULD TEST FOR SATURATIO HERE!!
    /// WE SHOULD TEST FOR SATURATIO HERE!!
    /// WE SHOULD TEST FOR SATURATIO HERE!!
    /// WE SHOULD TEST FOR SATURATIO HERE!!
    /// WE SHOULD TEST FOR SATURATIO HERE!!


                    for( size_t i = 0; i < dataStruct.imgParam.numY; ++i )
                        for( size_t j = 0; j < dataStruct.imgParam.numX; ++j )
                            procData[ i * dataStruct.imgParam.numX + j ] =  (double) data[  ( dataStruct.imgParam.ystart + i ) * dataStruct.imgParam.numPixX +  ( dataStruct.imgParam.xstart + j )    ];
                    dataIsCut = true;
                    ret = dataIsCut;
                }
                break;
            case velaCamStructs::k_BACK:
                if( BackIsSet() )
                {
//                    std::cout<< "cut k_BACKGROUND ";
                    procBack.clear();
                    procBack.resize( dataStruct.imgParam.numX * dataStruct.imgParam.numY ); /// set in positiveCutValues();
                    for( size_t i = 0; i < dataStruct.imgParam.numY; ++i )
                    {
                        for( size_t j = 0; j < dataStruct.imgParam.numX; ++j )
                        {
                            procBack[ i * dataStruct.imgParam.numX + j ] =  (double) back[  ( dataStruct.imgParam.ystart + i ) * dataStruct.imgParam.numPixX +  ( dataStruct.imgParam.xstart + j )    ];
                        }
                    }
                    backIsCut = true;
                    ret = backIsCut;
                }
                break;
        }
    }
    else // if( positiveCutValues() )
    {
        std::cout<< "positiveCutValues() is false " << std::endl;
    }
   // std::cout<< ".... FIN" << std::endl;
    return ret;
}
//______________________________________________________________________________
bool cameraDataObject::applyMask( velaCamStructs::DATA_TYPE type )
{
    bool ret     = false;
    bool carryon = true;

    if( MaskNotSet() )
    {
        std::cout << "Mask is not set " << std::endl;
        carryon = getMask();

        std::cout << "carryon after getMask() " << carryon << std::endl;
    }


    if( carryon )
    {
        switch( type )
        {
            case velaCamStructs::k_DATA:
                if( DataIsSet() )
                {
                    if( DataIsNotCut() )
                        carryon = cut( velaCamStructs::k_DATA );
                    if( carryon )
                    {
                        //std::cout << "APPLYIGN MASK " << std::endl;
                        std::transform(mask.begin(), mask.end(), procData.begin(), procData.begin(), std::multiplies<double>());
                    }
                    ret = true;
                }
                break;
            case velaCamStructs::k_BACK:
                if( BackIsSet() )
                {
                    if( BackIsNotCut() )
                        carryon = cut( velaCamStructs::k_BACK );
                    if( carryon )
                        std::transform(mask.begin(), mask.end(), procBack.begin(), procBack.begin(), std::plus<double>());
                    ret = true;
                }
                break;
        }
    }
    return ret;
}
//______________________________________________________________________________






//______________________________________________________________________________
bool cameraDataObject::getMask()
{
    maskSet = false;
    if( positiveCutValues() )
    {
        std::cout << "Calculating MASK " << std::endl;

        mask.clear();
        mask.resize( dataStruct.imgParam.numX * dataStruct.imgParam.numY );

        /// the exact centre of the mask is { h, k }

        double h, k;

        if( dataStruct.imgParam.numX % 2 == 0  )
            h = (double) dataStruct.imgParam.numX / 2.0;        // even
        else
            h = (double) dataStruct.imgParam.numX / 2.0 - 0.5;  // odd

        if( dataStruct.imgParam.numY % 2 == 0  )
            k = (double) dataStruct.imgParam.numY / 2.0;        // even
        else
            k = (double) dataStruct.imgParam.numY / 2.0 - 0.5;  // odd

        for( size_t i = 0; i < dataStruct.imgParam.numY; ++i )
        {
            for( size_t j = 0; j < dataStruct.imgParam.numX; ++j )
            {
                if( inEllipse( i , j , h, k) )
                    mask[ i * dataStruct.imgParam.numX + j ] = 1.0;
                else
                    mask[ i * dataStruct.imgParam.numX + j ] = 0.0;
            }
        }
        maskSet = true; /// now we do have a mask
    }
    else
    {
        std::cout << "Can't calculate mask, cut values error " << std::endl;
    }
    return maskSet;
}
//______________________________________________________________________________
bool cameraDataObject::inEllipse( size_t xCoord, size_t yCoord, double h, double k)
{
    /// h and k are real as they are the position, in pixels,
    /// of the centre of the pixel
    /// This function determines if {h, k} is within the ellipse (already centred at { x,0, y0 }

    if( fabs( (double) xCoord - h ) > dataStruct.imgParam.xRad   )
        return false;
    if( fabs( (double) yCoord - k ) > dataStruct.imgParam.yRad   )
        return false;
    if(  ( pow( ( (double) xCoord - h ) / (double)  dataStruct.imgParam.xRad, 2.0 ) + pow( ( (double)  yCoord - k ) / (double) dataStruct.imgParam.yRad, 2.0 ) ) > 1 )
        return false;
    else
        return true;
}
//______________________________________________________________________________
bool cameraDataObject::positiveCutValues()
{
    bool ret = true;
    if( dataStruct.imgParam.x0   < 1 ) ret = false;
    if( dataStruct.imgParam.y0   < 1 ) ret = false;
    if( dataStruct.imgParam.xRad < 1 ) ret = false;
    if( dataStruct.imgParam.yRad < 1 ) ret = false;

    /// the following are just to make the cutting of the
    /// raw data down to the screen more readable
    /// and a sanity check on the values

    //std::cout<< "positiveCutValues() is  " << ret <<  std::endl;



    if( ret )
    {
        dataStruct.imgParam.xstart = dataStruct.imgParam.x0 - dataStruct.imgParam.xRad;
        dataStruct.imgParam.xstop  = dataStruct.imgParam.x0 + dataStruct.imgParam.xRad;
        dataStruct.imgParam.ystart = dataStruct.imgParam.y0 - dataStruct.imgParam.yRad;
        dataStruct.imgParam.ystop  = dataStruct.imgParam.y0 + dataStruct.imgParam.yRad;

        if( dataStruct.imgParam.xstart > dataStruct.imgParam.xstop )        /// sanity check
            ret = false;
        else if( dataStruct.imgParam.ystart > dataStruct.imgParam.ystop )   /// sanity check
            ret = false;
        else
        {
            dataStruct.imgParam.numX = ( dataStruct.imgParam.xstop - dataStruct.imgParam.xstart );
            dataStruct.imgParam.numY = ( dataStruct.imgParam.ystop - dataStruct.imgParam.ystart );
        }
    }
    return ret;
}
//______________________________________________________________________________
bool cameraDataObject::testForSaturation( )
{
//        switch( type )
//        {
//            case velaCamStructs::k_DATA:
//                if( DataIsSet() )
//                {
//                    if( DataIsNotCut() )
//                        carryon = cut( velaCamStructs::k_DATA );
//                    if( carryon )
//                    {
//                        //std::cout << "APPLYIGN MASK " << std::endl;
//                        std::transform(mask.begin(), mask.end(), procData.begin(), procData.begin(), std::multiplies<double>());
//                    }
//                    ret = true;
//                }
//                break;
//            case velaCamStructs::k_BACK:
//                if( BackIsSet() )
//                {
//                    if( BackIsNotCut() )
//                        carryon = cut( velaCamStructs::k_BACK );
//                    if( carryon )
//                        std::transform(mask.begin(), mask.end(), procBack.begin(), procBack.begin(), std::plus<double>());
//                    ret = true;
//                }
//                break;
//        }
//
//
//
//    double minV = *std::min_element( procData.begin(), procData.end() );
//    double maxV = *std::max_element( procData.begin(), procData.end() );
//    int  maxC = std::count(procData.begin(), procData.end(), maxV);
//    double minC = std::count(procData.begin(), procData.end(), minV);
//    std::cout << "min = " << minV << " count = " << minC << ", max = " << maxV << " count = " << maxC << std::endl;
//    int minNumForStauration = 100; /// MAGIIC NUMBER
//    dataIsSaturated =  maxC > minNumForStauration ? true : false;
    return false;
}
//______________________________________________________________________________
/// _______ __
///|    ___|  |.---.-.-----.-----.
///|    ___|  ||  _  |  _  |__ --|
///|___|   |__||___._|___  |_____|
///                  |_____|
///
/// These flags are used to sya if there is data, backgrounds, cut data etc.
//______________________________________________________________________________
void cameraDataObject::setDataIsSet( bool v )
{
    dataIsSet = v;
}
//______________________________________________________________________________
void cameraDataObject::setDataIsCut( bool v )
{
    dataIsCut = v;
}
//______________________________________________________________________________
void cameraDataObject::setBackIsSet( bool v )
{
    backIsSet = v;
}
//______________________________________________________________________________
bool cameraDataObject::DataIsSet()
{
    return dataIsSet;
}
//______________________________________________________________________________
bool cameraDataObject::DataIsNotSet()
{
    return !dataIsSet;
}
//______________________________________________________________________________
bool cameraDataObject::DataIsCut()
{
    return dataIsCut;
}
//______________________________________________________________________________
bool cameraDataObject::DataIsNotCut()
{
    return !dataIsCut;
}
//______________________________________________________________________________
bool cameraDataObject::BackIsSet()
{
    return backIsSet;
}
//______________________________________________________________________________
bool cameraDataObject::BackIsNotSet()
{
    return !backIsSet;
}
//______________________________________________________________________________
bool cameraDataObject::BackIsCut()
{
    return backIsCut;
}
//______________________________________________________________________________
bool cameraDataObject::BackIsNotCut()
{
    return !backIsCut;
}
//______________________________________________________________________________
bool cameraDataObject::MaskSet()
{
    return maskSet;
}
//______________________________________________________________________________
bool cameraDataObject::MaskNotSet()
{
    return !maskSet;
}
//______________________________________________________________________________
bool cameraDataObject::setDataToDebug()
{
    bool ret = true;
    data.clear();
    data.resize( 1392 * 1040 ); /// MAGIC NUMBER, OK AS WE READ IN SAME FILE
    const char * fileName;
#ifdef _WINDOWS_MACHINE
	fileName = "C:\\Users\\djs56\\Documents\\VELA\\c++\\Applications\\cameraViewer\\data\\background1.dat";
#else
    fileName = "/home/console/djs/cameraViewer/data/background1.dat" ;
#endif
    std::ifstream inputFile;
    inputFile.open( fileName, std::ios::binary );
    if( inputFile )
        inputFile.read( (char*)&data[0], sizeof( unsigned char ) * data.size() );
    else
    {
        std::cout << "DATA READING FAILED"<< std::endl;
        ret = false;
    }
    return ret;
}
