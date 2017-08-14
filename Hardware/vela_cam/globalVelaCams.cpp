#include "globalVelaCams.h"

//#include "globalVelaScreens.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>

namespace globalVelaCams
{
    const size_t numCams = 13;
    const size_t standardNumPixX = 1392;
    const size_t standardNumPixY = 1040;
    const size_t standardTotPix  = standardNumPixX * standardNumPixY;

    const size_t getNumPix()
    {
        return standardTotPix;
    }

    const velaCamStructs::imgCutStruct dummyImgCutStruct = { -999, -999, -999, -999, 0,0,0,0,0,0,0,0,0, -999.99, -999.99};

    const std::string noEpicsName = "NO NAME";

    const std::vector< std::string > camNames      = getCamNames();
    const std::vector< std::string > camServerNums = getCamServerNums();
    std::map< std::string, velaCamStructs::imgCutStruct > imgCutParamMap = getImgCutParamMap();
//    const std::vector< std::string > camServerPVs  = getCamServerPVs();
//    const std::vector< std::string > camDataPVs    = getCamDataPVs();


    const std::vector< std::string > getCamNames()
    {
        return getScreenNames();
    }

    const std::vector< std::string > getScreenNames()
    {
        return globalVelaScreens::screenNames;
    }


    const std::map< std::string, std::string > getCamServerNumMap()
    {
//        std::map < std::string, std::string > m;
//        std::vector< std::string >::const_iterator i, j;
//        for( i = camNames.begin(), j = camServerNums.begin(); i != camNames.end() && j != camServerNums.end(); ++i, ++j)
//            m[ *i ] = *j;
        std::map < std::string, std::string > m;
        m[ globalVelaScreens::getVC()     ] = "S140360" ;
        m[ globalVelaScreens::getYAG01()  ] = "S140606" ;
        m[ globalVelaScreens::getYAG02()  ] = "S140674" ;
        m[ globalVelaScreens::getYAG03()  ] = "S140594" ;
        m[ globalVelaScreens::getYAG04()  ] = "S140904" ;
        m[ globalVelaScreens::getYAG05()  ] = "S140358" ;
        m[ globalVelaScreens::getYAG06()  ] = "S141195" ;
        m[ globalVelaScreens::getYAG07()  ] = "S140682" ;
        m[ globalVelaScreens::getYAG08()  ] = "S140682" ;
        //m[ globalVelaScreens::getYAGBA1() ] = "EBT-CS-CS4A-IOC-10" ; "S140812"
        m[ globalVelaScreens::getYAG09()  ] = "S140556" ;
        m[ globalVelaScreens::getYAGBA2() ] = "S140843" ;
        m[ globalVelaScreens::getED1()    ] = "S140845" ;
        m[ globalVelaScreens::getED2()    ] = "S140842" ;

        return m;
    }

    const std::map< std::string, std::string > getCamServerPVMap()
    {
        std::map < std::string, std::string > m;
        m[ globalVelaScreens::getVC()     ] = "EBT-CS-CS4A-IOC-06" ;
        m[ globalVelaScreens::getYAG01()  ] = "EBT-CS-CS4A-IOC-03" ;
        m[ globalVelaScreens::getYAG02()  ] = "EBT-CS-CS4A-IOC-04" ;
        m[ globalVelaScreens::getYAG03()  ] = "EBT-CS-CS4A-IOC-05" ;
        m[ globalVelaScreens::getYAG04()  ] = "EBT-CS-CS4A-IOC-01" ;
        m[ globalVelaScreens::getYAG05()  ] = "EBT-CS-CS4A-IOC-07" ;
        m[ globalVelaScreens::getYAG06()  ] = "EBT-CS-CS4A-IOC-02" ;
        m[ globalVelaScreens::getYAG07()  ] = "EBT-CS-CS4A-IOC-08" ;
        m[ globalVelaScreens::getYAG08()  ] = "EBT-CS-CS4A-IOC-09" ;
        //m[ globalVelaScreens::getYAGBA1() ] = "EBT-CS-CS4A-IOC-10" ;
        m[ globalVelaScreens::getED1()    ] = "EBT-CS-CS4A-IOC-11" ;
        m[ globalVelaScreens::getED2()    ] = "EBT-CS-CS4A-IOC-12" ;
        m[ globalVelaScreens::getYAG09()  ] = "EBT-CS-CS4A-IOC-13" ;
        m[ globalVelaScreens::getYAGBA2() ] = "EBT-CS-CS4A-IOC-14" ;
        return m;
    }

    const std::map< std::string, std::string > getCamDataPVMap()
    {
        std::map < std::string, std::string > m;
        m[ globalVelaScreens::getVC()     ] = "EBT-INJ-DIA-CAM-06:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAG01()  ] = "EBT-INJ-DIA-CAM-02:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAG02()  ] = "EBT-INJ-DIA-CAM-03:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAG03()  ] = "EBT-INJ-DIA-CAM-05:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAG04()  ] = "EBT-INJ-DIA-CAM-01:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAG05()  ] = "EBT-INJ-DIA-CAM-07:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAG06()  ] = "EBT-INJ-DIA-CAM-02:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAG07()  ] = "EBT-INJ-DIA-CAM-08:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAG08()  ] = "EBT-INJ-DIA-CAM-09:CAM:ArrayData" ;
        //m[ globalVelaScreens::getYAGBA1() ] = "EBT-INJ-DIA-CAM-10:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAG09()  ] = "EBT-INJ-DIA-CAM-13:CAM:ArrayData" ;
        m[ globalVelaScreens::getYAGBA2() ] = "EBT-INJ-DIA-CAM-14:CAM:ArrayData" ;
        m[ globalVelaScreens::getED1()    ] = "EBT-INJ-DIA-CAM-11:CAM:ArrayData" ;
        m[ globalVelaScreens::getED2()    ] = "EBT-INJ-DIA-CAM-12:CAM:ArrayData" ;

        return m;
    }

    extern velaCamStructs::imgCutStruct getImgCutParam( std::string screenName )
    {
//        std::map< std::string, velaCamStructs::imgCutStruct > m = getImgCutParamMap();
        if( imgCutParamMap.count( screenName ) )
            return imgCutParamMap[ screenName ];
        else
        {
//            std::cout << "gloablVela Cams is returning a dummyImgCutStruct" << std::endl;
            return dummyImgCutStruct;

        }
    }

    const std::vector< std::string > getCamServerNums()
    {
        std::vector< std::string > r;
        r.push_back( "S140681" );
        r.push_back( "S140360" );
        r.push_back( "S140606" );
        r.push_back( "S140674" );
        r.push_back( "S140594" );
        r.push_back( "S140904" );
        r.push_back( "S140358" );
        r.push_back( "S141195" );
        r.push_back( "S140682" );
        r.push_back( "S140812" );
        r.push_back( "S140845" );
        r.push_back( "S140842" );
        return r;
    }
    const std::vector< std::string > getCamServerPVs()
    {
        std::vector< std::string > r;
        r.push_back( "EBT-CS-CS4A-IOC-01" );
        r.push_back( "EBT-CS-CS4A-IOC-02" );
        r.push_back( "EBT-CS-CS4A-IOC-03" );
        r.push_back( "EBT-CS-CS4A-IOC-04" );
        r.push_back( "EBT-CS-CS4A-IOC-05" );
        r.push_back( "EBT-CS-CS4A-IOC-06" );
        r.push_back( "EBT-CS-CS4A-IOC-07" );
        r.push_back( "EBT-CS-CS4A-IOC-08" );
        r.push_back( "EBT-CS-CS4A-IOC-09" );
        r.push_back( "EBT-CS-CS4A-IOC-10" );
        r.push_back( "EBT-CS-CS4A-IOC-11" );
        r.push_back( "EBT-CS-CS4A-IOC-12" );
        r.push_back( "EBT-CS-CS4A-IOC-13" );
        r.push_back( "EBT-CS-CS4A-IOC-14" );
        return r;
    }

    const std::vector< std::string > getCamDataPVs()
    {
        std::vector< std::string > r;
        r.push_back( "EBT-INJ-DIA-CAM-01:CAM:ArrayData" );
        r.push_back( "EBT-INJ-DIA-CAM-02:CAM:ArrayData" );
        r.push_back( "EBT-INJ-DIA-CAM-03:CAM:ArrayData" );
        r.push_back( "EBT-INJ-DIA-CAM-04:CAM:ArrayData" );
        r.push_back( "EBT-INJ-DIA-CAM-05:CAM:ArrayData" );
        r.push_back( "EBT-INJ-DIA-CAM-06:CAM:ArrayData" );
        r.push_back( "EBT-INJ-DIA-CAM-07:CAM:ArrayData" );
        r.push_back( "EBT-INJ-DIA-CAM-08:CAM:ArrayData" );
        return r;
    }

    const std::map< std::string, velaCamStructs::imgCutStruct > getImgCutParamMap()
    {
        const char * fileName;
    #ifdef _WINDOWS_MACHINE
        fileName = "C:\\Users\\djs56\\Documents\\VELA\\c++\\Hardware\\camera\\CameraImageCuts.txt";
    #else
        fileName = "/home/console/djs/c++/Hardware/camera/CameraImageCuts.txt";
    #endif
        /// The following are switches that decide which type of data from the

        bool readingFile = false;

        std::string line, part1, part2;
        std::ifstream inputFile;
        std::map< std::string, velaCamStructs::imgCutStruct > m;
        velaCamStructs::imgCutStruct dummy;

        std::cout <<  std::endl;
        std::cout << "\t*** globalVelaCams is Reading in Camera Image Cut Parameters ***" << std::endl;
        std::cout << fileName << std::endl;
        std::cout <<  std::endl;
        inputFile.open( fileName, std::ios::in);
        if( inputFile )
        {
            std::cout << "\tSTART READING FILE" << std::endl;
            while( std::getline( inputFile, line ) ) /// Go throuh line by line,
            {
                //std::cout << "line = " <<  line <<std::endl;

				std::stringstream iss( line ); /// make a stream of the line and then do some tests

                /// Check if "END OF FILE" or "DATA START" has been reached.
                if( iss.str().find( "DATA START" ) != std::string::npos )
                {
                    std::cout << "\tFOUND 'START OF DATA'" << std::endl;
                    readingFile = true;
                }
                else if( iss.str().find("END OF FILE" ) != std::string::npos )
                {
                    std::cout << "\tFOUND 'END OF FILE', Data Read in Successfully" << std::endl;
                    std::cout << std::endl;
                    readingFile = false;
                }
                if( readingFile ) /// If still reading file, check what is written in the line.
                {
                    part1.clear();
                    part2.clear();
                    if( std::getline( iss, part1 , ':') ) /// put up to the first colon in part1
                    {
                        if( std::getline( iss, part2 , ':') );/// put up to the 2nd colon in part2
                    }
                    std::cout << "part1 = " << part1 << std::endl;
                    std::cout << "part2 = " << part2 << std::endl;
                    if ( part1.find("SCREEN") != std::string::npos  )
                    {
                        /// read in the next line, it contains the numbers;

						m[ part2 ];

                        std::getline( inputFile, line );
                        std::stringstream iss( line );


						//std::cout << "iss.str() = " << iss.str() << std::endl;

                        /// Put the numbers into the dummy imgCutStruct

                        //iss >> dummy.x0  >> dummy.y0 >> dummy.xRad >> dummy.yRad >> dummy.xstart>> dummy.xstop>> dummy.ystart>> dummy.ystop
                        //>> dummy.numPixX>> dummy.numPixY>> dummy.totNumPix>> dummy.numX>> dummy.numY>> dummy.xPix2mm>> dummy.yPix2mm;

						iss >> m[ part2 ].x0  >> m[ part2 ].y0 >> m[ part2 ].xRad >> m[ part2 ].yRad >> m[ part2 ].xstart>> m[ part2 ].xstop>> m[ part2 ].ystart>> m[ part2 ].ystop
                        >> m[ part2 ].numPixX>> m[ part2 ].numPixY>> m[ part2 ].totNumPix>> m[ part2 ].numX>> m[ part2 ].numY>> m[ part2 ].xPix2mm>> m[ part2 ].yPix2mm;


                        /// add to the ImgCutParamMap with key part2 (screen name)

						std::cout << "m[ part2 ].x0 " << m[ part2 ].x0 << std::endl;

                        calcICSStartAnsStops( m[ part2 ] );
                        //m[ part2 ] = dummy;

						std::cout <<"m[ part2 ].x0" << m[ part2 ].x0 << std::endl;

//                        std::cout << "dummy.x0 = " << dummy.x0 << std::endl;
//                        std::cout << "dummy.y0 = " << dummy.y0 << std::endl;
//                        std::cout << "dummy.xRad = " << dummy.xRad << std::endl;
//                        std::cout << "dummy.yRad = " << dummy.yRad << std::endl;
//                        std::cout << "dummy.xstart = " << dummy.xstart << std::endl;
//                        std::cout << "dummy.xstop = " << dummy.xstop << std::endl;
//                        std::cout << "dummy.ystart = " << dummy.ystart << std::endl;
//                        std::cout << "dummy.ystop = " << dummy.ystop << std::endl;
//                        std::cout << "dummy.numPixX = " << dummy.numPixX << std::endl;
//                        std::cout << "dummy.numPixY = " << dummy.numPixY << std::endl;
//                        std::cout << "dummy.totNumPix = " << dummy.totNumPix << std::endl;
//                        std::cout << "dummy.numX = " << dummy.numX << std::endl;
//                        std::cout << "dummy.numY = " << dummy.numY << std::endl;
//                        std::cout << "dummy.xPix2mm = " << dummy.xPix2mm << std::endl;
//                        std::cout << "dummy.yPix2mm = " << dummy.yPix2mm << std::endl;



                    }// if "SCREEN"
                }// if readingFile
            }// while reading in lines

			            std::cout << "END FILE READING"  << std::endl;
        }// if file exists
        else
        {
            std::cout << "\t****WARNING globalVelaCams CAN NOT FIND CameraImageCuts.txt**** " << std::endl;
            std::cout << "\t****RETURNING DUMMY PARAMETERS****" << std::endl;
            std::cout << std::endl;
            m = getImgCutParamMapDummies();
        }


		std::cout <<"a big test m[ YAG06 ].x0" << m[ "YAG06" ].x0 << std::endl;

        return m;
    }

    const std::map< std::string, velaCamStructs::imgCutStruct > getImgCutParamMapDummies()
    {
            // int x0, y0, xRad, yRad;
            // size_t xstart, xstop, ystart, ystop, numPixX, numPixY, numX, numY;
            // double xPix2mm, yPix2mm;
        std::map< std::string, velaCamStructs::imgCutStruct > m;

        velaCamStructs::imgCutStruct dummy = { 720, 505, 290, 295, 0, 0, 0, 0, standardNumPixX, standardNumPixY, standardTotPix, 0, 0, 21.57, 12.35 };

        std::vector< std::string > screens = getScreenNames();
        std::vector< std::string >::iterator it ;


        for( it = screens.begin(); it != screens.end(); ++ it)
            m[ *it ] = dummy;

        return m;
    }


    void calcICSStartAnsStops( velaCamStructs::imgCutStruct & icsData )
    {
        /// c++ first element is zero

        icsData.x0 -= 1;
        icsData.y0 -= 1;

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





//
//

//
//
//
//
//    const std::map< std::string, std::string > getCamServerPVMap()
//    {
//        std::map < std::string, std::string > m;
//        std::vector< std::string >::const_iterator i, j;
//        for( i = camNames.begin(), j = camServerPVs.begin(); i != camNames.end() && j != camServerPVs.end(); ++i, ++j)
//            m[ *i ] = *j;
//        return m;
//    }
//
//    const std::map< std::string, std::string > getCamDataPVMap()
//    {
//        std::map < std::string, std::string > m;
//        std::vector< std::string >::const_iterator i, j;
//        for( i = camNames.begin(), j = camDataPVs.begin(); i != camNames.end() && j != camDataPVs.end(); ++i, ++j)
//            m[ *i ] = *j;
//        return m;
//    }
//
//    const size_t getNumPix()
//    {
//        return standardNumPixX * standardNumPixY;
//    }
//
}

