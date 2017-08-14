#include "beamSizeCalculator.h"
//djs
//#include "globalVelaScreens.h"
//#include "globalVelaCams.h"
//#include "globalVelaMags.h"
#include "cameraImageCropper.h"
//#include "UTL.h"
// root
//#include "TGWindow.h"
#include "TSystem.h"
#include "TThread.h"
#include "TObjArray.h"
#include "TH2C.h"
#include "TH1D.h"
#include "TF1.h"
#include "TF2.h"
#include "TGraph2D.h"
#include "TCanvas.h"
#include <TFrame.h>
#include <TMath.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>

// stl
#include <iostream>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <numeric>
#include <iterator>

beamSizeCalculator::beamSizeCalculator() : imageCropper( NULL ) , shouldGetCommonestFLAG( false ),
shouldSubtractCommonestFLAG( false ), haveDataToFit( false ), fitHist( NULL ), satValue( 255 )
{
    imageCropper = new cameraImageCropper();
    //UTL::setColorScheme();

    bvnFunction = new TF2("bvnFunction", beamSizeCalculator::bvnExpression, 0, 1, 0, 1, 7);
    bvnFunction -> SetContour( 5 );
    bvnFunction -> SetParName( 0, "BVN_Bac" );
    bvnFunction -> SetParName( 1, "BVN_Amp" );
    bvnFunction -> SetParName( 2, "BVN_mux" );
    bvnFunction -> SetParName( 3, "BVN_muy" );
    bvnFunction -> SetParName( 4, "BVN_sxx" );
    bvnFunction -> SetParName( 5, "BVN_syy" );
    bvnFunction -> SetParName( 6, "BVN_sxy" );
}
//______________________________________________________________________________
beamSizeCalculator::~beamSizeCalculator()
{
    if( imageCropper )
        delete imageCropper;
}






//______________________________________________________________________________
void beamSizeCalculator::cropRawDataToScreen()
{
    /// !!!! WE ASSUME THE SAME SCREEN FOR ALL DATA !!! rawData.screenNames[0]
    /// THIS IS FOR VERSON ONE OF THE DATA FILE, WILL PROBABLY NEED UPDATING

    beamDatCroppedToScreen.resize( rawData.dataWithBeam.size() );
    backDatCroppedToScreen.resize( rawData.dataWithNoBeam.size()  );

    for( size_t i = 0 ; i < beamDatCroppedToScreen.size(); ++i )//beamSizeData.cutDataWithBeam.size(); ++i  )
        beamDatCroppedToScreen[ i ] = imageCropper -> cropImageToScreen( rawData.dataWithBeam[i], rawData.screenNames[0] );

    for( size_t i = 0 ; i < backDatCroppedToScreen.size(); ++i )//beamSizeData.cutDataWithBeam.size(); ++i  )
        backDatCroppedToScreen[ i ] = imageCropper -> cropImageToScreen( rawData.dataWithNoBeam[i], rawData.screenNames[0] );


    if( backDatCroppedToScreen.size() == 0 || shouldGetCommonest() )
    {
        croppedCommenstValues.clear();
        croppedCommenstValues.resize( beamDatCroppedToScreen.size() );
        for( size_t i = 0 ; i < croppedCommenstValues.size(); ++i )//beamSizeData.cutDataWithBeam.size(); ++i  )
        {
            croppedCommenstValues[ i ] = getMostCommonElement( beamDatCroppedToScreen[0].data.begin(), beamDatCroppedToScreen[0].data.end());
            std::cout << "croppedCommenstValues[ " <<  i << " ] = " << (int)croppedCommenstValues[ i ] << std::endl;
        }
        if( shouldSubtractCommonest() && croppedCommenstValues.size() == beamDatCroppedToScreen.size() )
        {
            std::cout << std::endl;
            std::cout << "Subtracting commonest values" << std::endl;
            std::cout << std::endl;

            /// Actually this is pretty dangerous, as we are probably taking some unsigend char values out of range!

            std::vector< unsigned char >::iterator it;
            for( size_t i = 0; i < beamDatCroppedToScreen.size(); ++i )
            {
                for( it = beamDatCroppedToScreen[ i ].data.begin(); it != beamDatCroppedToScreen[ i ].data.end(); ++it )
                {
                    if( *it >= croppedCommenstValues[i] )
                        *it  -= croppedCommenstValues[i];
                    else
                        *it = 0;
                }
            }
        }
    }
/// If  plotting
//    int yB = beamDatCroppedToScreen[0].numY;
//    int xB = beamDatCroppedToScreen[0].numX;
//    TH2C * datCrop2ScreenH = new TH2C("datCrop2ScreenH","Cropped Data", xB, 0, xB, yB, 0, yB );
//    std::cout << "datCrop2ScreenH sum = " << std::accumulate(beamDatCroppedToScreen[0].data.begin(),beamDatCroppedToScreen[0].data.end(),0) << std::endl;
//    for( size_t y = 0; y < yB; ++y )
//        for( size_t x = 0; x < xB; ++x )
//            datCrop2ScreenH -> SetBinContent( x, yB - y - 1 , beamDatCroppedToScreen[0].data[ y * xB + x ] );
//    TCanvas *datCrop2ScreenHCan = new TCanvas("datCrop2ScreenHCan","Data Cropped To Screen",200,10,700,500);
//    datCrop2ScreenH -> Draw("COL");
}
//
void beamSizeCalculator::applyMask()
{
    std::vector< velaCamStructs::imgDatC >::iterator it;
    for( it = beamDatCroppedToScreen.begin(); it != beamDatCroppedToScreen.end(); ++it )
        imageCropper -> applyMask( it->data, rawData.screenNames[0] );
    for( it = backDatCroppedToScreen.begin(); it != backDatCroppedToScreen.end(); ++it )
        imageCropper -> applyMask( it->data, rawData.screenNames[0] );
///PLOT
//    int yB = beamDatCroppedToScreen[0].numY;
//    int xB = beamDatCroppedToScreen[0].numX;
//    TH2C * datCropMask2HC = new TH2C("datCropMask2HC","datCropMask2HC graph", xB, 0, xB, yB, 0, yB );
//    std::cout << "datCropMask2HC sum = " << std::accumulate(beamDatCroppedToScreen[0].data.begin(),beamDatCroppedToScreen[0].data.end(),0) << std::endl;
//    for( size_t y = 0; y < yB; ++y )
//        for( size_t x = 0; x < xB; ++x )
//            datCropMask2HC -> SetBinContent( x, yB - y - 1, beamDatCroppedToScreen[0].data[ y * xB + x ] );
//    TCanvas *datCropMask2HCCan = new TCanvas("datCropMask2HCCan","Data Cropped To Screen With Mask ",200,10,700,500);
//    datCropMask2HC -> Draw("COL");
}
//______________________________________________________________________________
void beamSizeCalculator::subtractBackGround()
{
    dataMinusBackGround.resize( beamDatCroppedToScreen.size() );
    if( backDatCroppedToScreen.size() > 0  ) /// There is background data
    {
//        calcMeanBackground();
        for( size_t i = 0; i < dataMinusBackGround.size(); ++i  )
        {
            dataMinusBackGround[i].data.resize( meanBackGround.data.size() );
            dataMinusBackGround[i].numX = beamDatCroppedToScreen[i].numX;
            dataMinusBackGround[i].numY = beamDatCroppedToScreen[i].numY;
            for( size_t j = 0; j < meanBackGround.data.size(); ++j )
                dataMinusBackGround[i].data[j] = (double)beamDatCroppedToScreen[i].data[j] - meanBackGround.data[j];
        }
    }
    else /// there is no background
    {
        std::cout << "No background to subtract" << std::endl;
        for( size_t i = 0 ; i < backDatCroppedToScreen.size(); ++i )
        {
            dataMinusBackGround[ i ].data.assign(beamDatCroppedToScreen[i].data.begin(), beamDatCroppedToScreen[i].data.end());
            dataMinusBackGround[i].numX = beamDatCroppedToScreen[i].numX;
            dataMinusBackGround[i].numY = beamDatCroppedToScreen[i].numY;
        }
    }
/// If plotting
//    int yB = dataMinusBackGround[0].numY;
//    int xB = dataMinusBackGround[0].numX;
//    TH2D * datCropMaskMinBack2HC = new TH2D("datCropMaskMinBack2HC","Cropped and Masked Data - Background ", xB, 0, xB, yB, 0, yB );
//    std::cout << "datCropMaskMinBack2HC sum = " << std::accumulate(dataMinusBackGround[0].data.begin(),dataMinusBackGround[0].data.end(),0) << std::endl;
//    for( size_t y = 0; y < yB; ++y )
//        for( size_t x = 0; x < xB; ++x )
//            datCropMaskMinBack2HC -> SetBinContent( x, y, dataMinusBackGround[0].data[ y * xB + x ] );
//    TCanvas *datCropMaskMinBack2HCCan = new TCanvas("datCropMaskMinBack2HCCan","cut mask, min back graph",200,10,700,500);
//    datCropMaskMinBack2HC -> Draw("COL");
//    plot1DProj( dataMinusBackGround[0] );
}
//______________________________________________________________________________
void beamSizeCalculator::cropTo2FWHM()
{
    velaCamStructs::imageCutStruct tempICSData;

    size_t cen, rad;
    dataToFit.reserve( dataMinusBackGround.size() );
    std::vector< velaCamStructs::imgDatD >::iterator it;
    for( it = dataMinusBackGround.begin(); it != dataMinusBackGround.end(); ++it )//dataMinusBackGround.size(); ++i  )
    {
        cut2FWHM( imageCropper -> getXProj( *it ) , tempICSData.x0, tempICSData.xRad );
        cut2FWHM( imageCropper -> getYProj( *it ) , tempICSData.y0, tempICSData.yRad );
        tempICSData.numPixX = it -> numX;
        tempICSData.numPixY = it -> numY;
        dataToFit.push_back( imageCropper -> cropImage( it->data, tempICSData ) );
    }
    haveDataToFit = true;
/// If Plotting
//    int yB = dataToFit[0].numY;
//    int xB = dataToFit[0].numX;
//    TH2D * dataToFitTH2D = new TH2D("dataToFitTH2D","Cropped and Masked Data - Background ", xB, 0, xB, yB, 0, yB );
//    std::cout << "dataToFitTH2D sum = " << std::accumulate(dataToFit[0].data.begin(),dataToFit[0].data.end(),0) << std::endl;
//    for( size_t y = 0; y < yB; ++y )
//        for( size_t x = 0; x < xB; ++x )
//            dataToFitTH2D -> SetBinContent( x, y, dataToFit[0].data[ y * xB + x ] );
//    TCanvas *dataToFitTH2DCan = new TCanvas("dataToFitTH2DCan","cut 2D",200,10,700,500);
//    dataToFitTH2D -> Draw("COL");
//    plot1DProj( dataToFit[0] );
}
//______________________________________________________________________________
void beamSizeCalculator::fitBVNData()
{
    if( haveDataToFit )
    {
        if( dataToFit.size() == saturatedPositions.size() )
        {
            dataFits.reserve( dataToFit.size() );
            std::vector< velaCamStructs::imgDatD  >::iterator i;
            std::vector< std::vector< int > >::iterator j;
            for(i = dataToFit.begin(), j = saturatedPositions.begin(); i != dataToFit.end() && j != saturatedPositions.end(); ++i, ++j)
                dataFits.push_back( fitToBVN( *i, *j  ) );

        }
        else
        {
            std::cout << "dataToFit and saturatedPositions different size, fitting aborted" << std::endl;

        }
    }
}
//______________________________________________________________________________
velaCamStructs::bvnFit beamSizeCalculator::fitToBVN( velaCamStructs::imgDatD & ics, std::vector< int > & satPos    )
{
    std::cout << "***Fitting BVN ***" << std::endl;

    if( fitHist == NULL )
        fitHist = new TH2D("BVNFittingHist","BVN Fitting Histogram", ics.numX, 0, ics.numX, ics.numY, 0, ics.numY );
    else
        fitHist -> SetBins( ics.numX, 0, ics.numX, ics.numY, 0, ics.numY );

    /// To handle saturated points we make a GRAPH2D from the 2D hist, but first we set saturated bins to
    /// 0 and error to 0 so that "Empty bins, recognized when both content and errors are zero, are excluded."
    /// from https://root.cern.ch/root/html/TGraph2D.html#TGraph2D:TGraph2D@4

    for( size_t y = 0; y < ics.numY; ++y )
        for( size_t x = 0; x < ics.numX; ++x )
        {
            fitHist -> SetBinContent( x, y, ics.data[ y * ics.numX + x ] );
            if(std::find(satPos.begin(), satPos.end(), y * ics.numX + x) != satPos.end())
            {
                fitHist -> SetBinContent( x, y, 0.0 );
                fitHist -> SetBinError  ( x, y, 0.0 );
            }
        }
    TGraph2D graph = TGraph2D( fitHist );

    double minE = *std::min_element(std::begin(ics.data), std::end(ics.data) );
    double maxE = *std::max_element(std::begin(ics.data), std::end(ics.data) );
//    double sum_of_elems = (double)std::accumulate(ics.data.begin(),ics.data.end(),0);
//    std::cout << "Data Min = " << minE << ", Max = " << maxE << " sum = " << sum_of_elems << std::endl;

//    TCanvas *c2d = new TCanvas("c22","cut 2D",200,10,700,500);
//    c2d->SetGrid();
//    fitHist -> Draw("COL");

    bvnFunction -> SetRange( 0, ics.numX, 0, ics.numY );
    bvnFunction -> SetParameter( 0, 0.0 );
    bvnFunction -> SetParameter( 1, maxE - minE  );
    bvnFunction -> SetParameter( 2, fitHist -> GetMean( 1 ) );
    bvnFunction -> SetParameter( 3, fitHist -> GetMean( 2 ) );
    bvnFunction -> SetParameter( 4, TMath::Power( fitHist -> GetRMS( 1 ), 2.0 ) );
    bvnFunction -> SetParameter( 5, TMath::Power( fitHist -> GetRMS( 2 ), 2.0 )  );
    bvnFunction -> SetParameter( 6, 0.0  );

    /// for fitting options see ftp://root.cern.ch/root/doc/5FittingHistograms.pdf

    TFitResultPtr res = graph.Fit("bvnFunction","QNS");

    //res -> Print("V");

    velaCamStructs::bvnFit ret;

    ret.Bac = bvnFunction -> GetParameter( 0 );
    ret.Amp = bvnFunction -> GetParameter( 1 );
    ret.mux = bvnFunction -> GetParameter( 2 );
    ret.muy = bvnFunction -> GetParameter( 3 );
    ret.sxx = bvnFunction -> GetParameter( 4 );
    ret.syy = bvnFunction -> GetParameter( 5 );
    ret.sxy = bvnFunction -> GetParameter( 6 );

    ret.BacErr = bvnFunction -> GetParError( 0 );
    ret.AmpErr = bvnFunction -> GetParError( 1 );
    ret.muxErr = bvnFunction -> GetParError( 2 );
    ret.muyErr = bvnFunction -> GetParError( 3 );
    ret.sxxErr = bvnFunction -> GetParError( 4 );
    ret.syyErr = bvnFunction -> GetParError( 5 );
    ret.sxyErr = bvnFunction -> GetParError( 6 );

    ret.status = res;

    return ret;

}
//______________________________________________________________________________
double beamSizeCalculator::bvnExpression(double *coords, double *par)
{
    double mux, muy, sxx, syy, sxy, x, y;  /// The main 2D. BVN, fitting Function
                                           /// define
    mux = par[2];                          /// par[0] = BVN_BackGround
    muy = par[3];                          /// par[1] = BVN_Amplitude
    sxx = par[4];                          /// par[2] = BVN_Meanx
    syy = par[5];                          /// par[3] = BVN_Meany
    sxy = par[6];                          /// par[4] = BVN_sigmaxx
                                           /// par[5] = BVN_sigmayy
    x = coords[0];                         /// par[6] = BVN_sigmaxy
    y = coords[1];

    double denom = sxx * syy - TMath::Power( sxy, 2.0 );
    double A1 = y - muy;
    double A2 = y * sxx - muy * sxx - x * sxy + mux * sxy;
    double B1 = x - mux;
    double B2 = muy * sxy + x * syy - mux * syy - y * sxy;
    double expTerm = 0.5 * ( -1.0 * ( A1 * A2 ) / denom - ( B1 * B2 ) / denom );

    return par[0] + par[1] * TMath::Exp( expTerm );
}
//______________________________________________________________________________
void beamSizeCalculator::getSaturatedPositions()
{
    saturatedPositions.reserve( beamDatCroppedToScreen.size() );
    std::vector< velaCamStructs::imgDatC >::iterator it;
    for( it = beamDatCroppedToScreen.begin(); it != beamDatCroppedToScreen.end(); ++it )
        saturatedPositions.push_back( getSaturatedPositions( it->data, satValue ) );
}
//______________________________________________________________________________
std::vector< int > beamSizeCalculator::getSaturatedPositions( std::vector< unsigned char > & data, unsigned char  value )
{
    std::vector< int > ret;
    std::vector< unsigned char>::iterator it;
    for( it = data.begin(); it != data.end(); ++it)
        if( *it == value )
            ret.push_back( std::distance( data.begin(), it ) );
    return ret;
}




velaCamStructs::projectedMomentsStruct getProjectedMoments(  velaCamStructs::imgDatC )
{
    velaCamStructs::projectedMomentsStruct  ret;

    return ret;
}

velaCamStructs::projectedMomentsStruct getProjectedMoments(  velaCamStructs::imgDatD )
{
    velaCamStructs::projectedMomentsStruct  ret;

    return ret;
}




















void beamSizeCalculator::cut2FWHM( std::vector< double > & vec, int & cen, int & rad )
{
//    std::cout << "cut2FWHM()" << std::endl;

/// we're going to use iterators to find the max position, then increment and decrement them,
/// then use standard distance to get the cut centre and radius,
/// simple...
/// There is no error checking here!!!!

    std::vector< double >::iterator itAtMaxPos = std::max_element(std::begin(vec), std::end(vec) );
    std::vector< double >::iterator itAtMaxPosPlus = itAtMaxPos;
    std::vector< double >::iterator itAtMaxPosMinus = itAtMaxPos;

    double maxVal     = *itAtMaxPos;
    double halfMaxVal = *itAtMaxPos / 2.0;

//    std::cout << "max Val = " << maxVal << std::endl;
//    std::cout << "max Pos = " << (size_t)std::distance( vec.begin(), itAtMaxPos) << std::endl;

    while( *itAtMaxPosPlus > halfMaxVal && itAtMaxPosPlus != vec.end() )
        ++itAtMaxPosPlus;

//    std::cout << "Half max +     = " << *itAtMaxPosPlus << std::endl;
//    std::cout << "Half max + Pos = " << (size_t)std::distance( vec.begin(), itAtMaxPosPlus) << std::endl;

    while( *itAtMaxPosMinus  > halfMaxVal && itAtMaxPosMinus != vec.begin() )
        --itAtMaxPosMinus;

//    std::cout << "Half max -     = " << *itAtMaxPosMinus << std::endl;
//    std::cout << "Half max - Pos = " << (size_t)std::distance( vec.begin(), itAtMaxPosMinus) << std::endl;
//    std::cout << std::endl;
//    std::cout << std::endl;
//    std::cout << "Distances from Max Pos " << std::endl;
//    std::cout << std::endl;
//    std::cout << "Max pos to +  " << (size_t)std::distance( itAtMaxPos, itAtMaxPosPlus) << std::endl;
//    std::cout << "Max pos to -  " << (size_t)std::distance( itAtMaxPosMinus, itAtMaxPos) << std::endl;
//    std::cout << std::endl;
//    std::cout << std::endl;

    /// We Take twice the distance to halfMaxVal on either side

    int fwhmPlus  = (int)std::distance( itAtMaxPos, itAtMaxPosPlus  );
    int fwhmMinus = (int)std::distance( itAtMaxPosMinus, itAtMaxPos );

//    std::cout << "xmax2aPos = " << xmax2aPos << std::endl;

    /// We Take twice the distance to halfMaxVal on either side

    int maxPos = (int)std::distance( vec.begin(), itAtMaxPos) + 2 * fwhmPlus;
    int minPos = (int)std::distance( vec.begin(), itAtMaxPos ) - 2 * fwhmMinus;

//    std::cout << "maxPos = " << maxPos << std::endl;
//    std::cout << "minPos = " << minPos << std::endl;

    /// Now we will fill the passed values for the cen and rad.

    rad = (int)TMath::Nint( (maxPos - minPos) / 2 );
    cen = minPos + rad;
    //std::cout << "rad = " << rad << ", cen = " << cen << std::endl;
}

























void beamSizeCalculator::calcMeanBackground()
{
    std::cout << "calcMeanBackground()" << std::endl;

    meanBackGround.data.resize( backDatCroppedToScreen[0].data.size(), 0.0 );
    std::vector< velaCamStructs::imgDatC >::iterator it1;
    for( it1 = backDatCroppedToScreen.begin(); it1 != backDatCroppedToScreen.end(); ++it1  )
    {
        for( size_t i = 0; i < (*it1).data.size(); ++i )
            meanBackGround.data[i] += (double)(*it1).data[i];
    }
    std::vector< double >::iterator it2;
    for( it2 = meanBackGround.data.begin(); it2 != meanBackGround.data.end(); ++it2 )
        *it2 /=  backDatCroppedToScreen.size();
    meanBackGround.numX = backDatCroppedToScreen[0].numX;
    meanBackGround.numY = backDatCroppedToScreen[0].numY;
}

void beamSizeCalculator::plot1DProj( velaCamStructs::imgDatD & id )
{
    std::vector< double > xProj = imageCropper -> getXProj( id );
    std::vector< double > yProj = imageCropper -> getYProj( id );

    TH1D * yproj = new TH1D("yproj","y projections", id.numY, 0, id.numY );
    TH1D * xproj = new TH1D("xproj","x projections", id.numX, 0, id.numX );

    for( size_t y = 0; y < id.numY; ++y )
    {
        yproj -> SetBinContent( y, yProj[y] );
        //std::cout << "yProj[ " << y << " ] = " << yProj[y]  << std::endl;
    }

    for( size_t x = 0; x < id.numX; ++x )
    {
        xproj -> SetBinContent( x, xProj[x] );
    }

    TCanvas *c1 = new TCanvas("c1","Projections Example",200,10,700,500);

    c1->SetFillColor(21);
    c1->SetGrid();
    c1->Divide(2);

    c1->cd(1);
    yproj -> Draw();

    c1->cd(2);
    xproj -> Draw();

    // TCanvas::Update() draws the frame, after which one can change it
    c1->Update();
    c1->GetFrame()->SetFillColor(21);
    c1->GetFrame()->SetBorderSize(12);
    c1->Modified();


}



void beamSizeCalculator::fillBeamSizeDataStruct( emitDAQStructs::emitScanSaveDataStruct & dataStruct )
{
    /// We only do this to change the data again, making things both more complicated and
    /// easier ;o)
    /// unlike reading in, now we can just set vectors equal to one another, phew

    beamSizeData.screenNames = dataStruct.screenNames;
    beamSizeData.screenImgCutStructs.resize( beamSizeData.screenNames.size() );

    /// no zips so no iterators

    for( int i = 0; i < beamSizeData.screenNames.size(); ++i )
        beamSizeData.screenImgCutStructs[i] =  globalVelaCams::getImgCutParam( beamSizeData.screenNames[i] );

    beamSizeData.numMagnets = dataStruct.totalNumMags;
    beamSizeData.magNames = dataStruct.allMagNames;
    beamSizeData.magSIs.resize( beamSizeData.magNames.size() );

    for( int i = 0; i < dataStruct.allMagNames.size(); ++i )
    {
        if( dataStruct.allMagPSUStates[ i ] == velaMagStructs::k_ON )
            beamSizeData.magSIs[i] = dataStruct.allMagSIs[i];
        else
            beamSizeData.magSIs[i] = 0;
    }

    for( int i = 0; i < dataStruct.config.numSISteps.size(); ++i )
    {
        if( dataStruct.config.numSISteps[ i ] != 0 )
            beamSizeData.scanningMagnets.push_back( dataStruct.config.magNames[i] );
    }

    std::cout << "*** Magnets Being Scanned ***" << std::endl;
    for( size_t i = 0; i < beamSizeData.scanningMagnets.size(); ++i )
        std::cout << beamSizeData.scanningMagnets[i] << std::endl;
    std::cout << std::endl;

    /// So there is a problem here, screen names is jus ttyhe screen being used, but what if we use two screens?
    /// THE CODE IS GENERIC ENOUGH T ODO THAT, BUT ANAYLSING THE DATA AUTOMATICALLY BECOMES REALLY
    /// COMPLICATED, I PROBABLY HAVE TO BITE THE BULLET


    std::cout << "*** Screens Being used ***" << std::endl;
    for( size_t i = 0; i < beamSizeData.screenNames.size(); ++i )
        std::cout << beamSizeData.screenNames[i] << std::endl;
    std::cout << std::endl;

    std::cout << "*** Magnet SIs ***" << std::endl;
    for( size_t i = 0 ; i < beamSizeData.magNames.size(); ++i )
        std::cout << beamSizeData.magNames[i] << ", SI = " << beamSizeData.magSIs[i] << std::endl;
    std::cout << std::endl;

    /// background images, take an average

    std::vector< double > background(  dataStruct.dataWithNoBeam[0].size() );

//
//
//    for( dataStruct.dataWithNoBeam.size() )
//    {
//        std::transform(
//        dataStruct.dataWithNoBeam[i].begin(), dataStruct.dataWithNoBeam[i].end(),
//                procData.begin(), procData.begin(), std::multiplies<double>());
//    }
//    dataStruct.dataWithNoBeam[i]




    beamSizeData.cutDataWithBeam.resize( dataStruct.dataWithBeam.size() );
    std::cout << "Cropping beamSizeData.cutDataWithBeam = " <<  beamSizeData.cutDataWithBeam.size() << std::endl;
    for( size_t i = 0 ; i < 1; ++i)//beamSizeData.cutDataWithBeam.size(); ++i  )
    {
        /// should dataStruct.screenNames have each screen for each image?

//        beamSizeData.cutDataWithBeam[ i ] = imageCropper -> cropImage( dataStruct.dataWithBeam[i], dataStruct.screenNames[0] );
//
//        double sum_of_elems = std::accumulate(beamSizeData.cutDataWithBeam[ i ].begin(),beamSizeData.cutDataWithBeam[ i ].end(),0);
//        std::cout << "beamSizeData.cutDataWithBeam[ i ].size() = " << beamSizeData.cutDataWithBeam[ i ].size()  << std::endl;
//        std::cout << "Hash After Cropping = " << sum_of_elems << std::endl;
//
//
//
//
//
//        /// we crop again to 2FWHM
//
//
//        imgDatC testidC;
//
//        testidC.data = beamSizeData.cutDataWithBeam[ i ];
//        testidC.numX = beamSizeData.screenImgCutStructs[0].numX;
//        testidC.numY = beamSizeData.screenImgCutStructs[0].numY;
//
//
//        cut2FWHM( testidC );


//        imageCropper -> applyMask( beamSizeData.cutDataWithBeam[ i ], dataStruct.screenNames[0] );
//        sum_of_elems = std::accumulate(beamSizeData.cutDataWithBeam[ i ].begin(),beamSizeData.cutDataWithBeam[ i ].end(),0);
//        std::cout << "beamSizeData.cutDataWithBeam[ i ].size() = " << beamSizeData.cutDataWithBeam[ i ].size()  << std::endl;
//        std::cout << "Hash After Mask = " << sum_of_elems << std::endl;

    }
    std::cout << " beamSizeData  Cropped " << std::endl;
  //  fitTo1DProj( beamSizeData );


//    std::vector< std::vector< unsigned char > > cutDataWithBeam
}

unsigned char beamSizeCalculator::getMostCommonElement(std::vector< unsigned char >::iterator begin, std::vector< unsigned char >::iterator end)
{
    std::map<unsigned char, int> count;

    for (std::vector< unsigned char >::iterator it = begin; it != end; ++it)
        ++count[*it];

    return std::max_element(count.begin(), count.end(), by_second()) -> first;
}



void beamSizeCalculator::fitTo1DProj( emitDAQStructs::beamSizeDataStruct & bsd )
{
//
//    int xB = bsd.screenImgCutStructs[0].numX;
//    int yB = bsd.screenImgCutStructs[0].numY;
//
//
//    std::cout << "*** fitTo1DProj ***" << std::endl;
////const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup)
////const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins
//    TH2C * cut2D = new TH2C("f2d","f2d graph", xB, 0, xB, yB, 0, yB );
//
//        std::cout << " TH2C * cut2D" << std::endl;
//
//    for( size_t y = 0; y < yB; ++y )
//    {
//        for( size_t x = 0; x < xB; ++x )
//        {
//            cut2D -> SetBinContent( x, y, bsd.cutDataWithBeam[ 0 ][ y * xB + x ] );
//        }
//    }
//
//    double sum_of_elems = (double)std::accumulate(bsd.cutDataWithBeam[ 0 ].begin(),bsd.cutDataWithBeam[ 0 ].end(),0);
//
//
//
//
//
//
//
//    std::cout << "sum_of_elems = " << sum_of_elems << std::endl;
//
//    TCanvas *c22 = new TCanvas("c2","cut 2D",200,10,700,500);
//        c22->cd();
//    c22->SetGrid();
//    cut2D -> Draw("COL");
//
//
//    std::vector< double > yProj( yB, 0.0 );
//    std::vector< double > yProjv( yB, 0.0 );
//    std::vector< double > xProj( xB, 0.0 );
//    std::vector< double > xProjv( xB, 0.0 );
//
//    TH1D * yproj2 = new TH1D("yproj2","y proj", yB, 0, yB );
//    TH1D * xproj2 = new TH1D("xproj2","x proj", xB, 0, xB );
//
//
//
//
//    for( size_t y = 0; y < yB; ++y )
//    {
//        yProjv[y] = (double)y;
//        for( size_t x = 0; x < xB; ++x )
//        {
//            yProj[y] += (double)bsd.cutDataWithBeam[ 0 ][ y * xB + x ];
//            //yproj2 -> AddBinContent( y, (double)bsd.cutDataWithBeam[ 0 ][ y * xB + x] );
//        }
//        //yproj2 -> SetBinContent( yB -1 - y, yProj[y] );
//        yproj2 -> SetBinContent( y, yProj[y] );
//    }
//
//    for( size_t x = 0; x < xB; ++x )
//    {
//        xProjv[x] = (double)x;
//        for( size_t y = 0; y < yB; ++y )
//        {
//            xProj[ x ] += (double)bsd.cutDataWithBeam[ 0 ][ y * xB + x ];
//        }
//        //xproj2 -> SetBinContent( xB -1 - x, xProj[x] );
//        xproj2 -> SetBinContent( x, xProj[x] );
//        //std::cout << xProj[x] << std::endl;
//    }
//
////    TH1D * xproj2 = new TH1D( "xproj2", "x proj me", xB, &xProj[0] );
////    TH1D * yproj2 = new TH1D( "yproj2", "y proj me", yB, &yProj[0] );
//
//    TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);
//
//    c1->SetFillColor(21);
//    c1->SetGrid();
//    c1->Divide(2);
//
//    c1->cd(1);
//    yproj2 -> Draw();
//
//    c1->cd(2);
//    xproj2 -> Draw();
//
//    // TCanvas::Update() draws the frame, after which one can change it
//    c1->Update();
//    c1->GetFrame()->SetFillColor(21);
//    c1->GetFrame()->SetBorderSize(12);
//    c1->Modified();
//
//
//    TF1 *norm1D = new TF1("norm1D","[0]+[1]*TMath::Exp(-0.5*((x-[2])/[3])^2)");
//
//
//
//   norm1D -> SetParName( 0, "BackGround" );
//   norm1D -> SetParName( 1, "Amplitude" );
//   norm1D -> SetParName( 2, "Mean" );
//   norm1D -> SetParName( 3, "Sigma" );
//
//
//
//    double si = yproj2 -> GetRMS();
//    std::cout << "si = " << si << std::endl;
//
//    double mn = yproj2 -> GetMean();
//    std::cout << "mn = " << mn << std::endl;
//
//    double Bg2 = *std::min_element(std::begin(yProj), std::end(yProj) );
//    double A02 = *std::max_element(std::begin(yProj), std::end(yProj) ) - Bg2;
//    std::cout << "Bg2 = " << Bg2 << std::endl;
//    std::cout << "A02 = " << A02 << std::endl;
//
//
//   norm1D -> SetParameter( 0, Bg2 );
//   norm1D -> SetParameter( 1, A02 );
//   norm1D -> SetParameter( 2, mn    );
//   norm1D -> SetParameter( 3, si     );
//
//   //norm1D->SetParLimits(2, mn-10.0, mn+10.0);
//
//    yproj2 -> Fit("norm1D");
//
//    TF1 *norm1D2 = new TF1("norm1D2","[0]+[1]*TMath::Exp(-0.5*((x-[2])/[3])^2)");
//
//   norm1D2 -> SetParName( 0, "BackGround2" );
//   norm1D2 -> SetParName( 1, "Amplitude2" );
//   norm1D2 -> SetParName( 2, "Mean2" );
//   norm1D2 -> SetParName( 3, "Sigma2" );
//
//    double si2 = xproj2 -> GetRMS();
//    std::cout << "si2 = " << si2 << std::endl;
//
//    double mn2 = xproj2 -> GetMean();
//    std::cout << "mn2 = " << mn2 << std::endl;
//
//    double Bg22 = *std::min_element(std::begin(xProj), std::end(xProj) );
//    double A022 = *std::max_element(std::begin(xProj), std::end(xProj) ) - Bg22;
//    std::cout << "Bg22 = " << Bg22 << std::endl;
//    std::cout << "A022 = " << A022 << std::endl;
//
//   norm1D2 -> SetParameter( 0, Bg22 );
//   norm1D2 -> SetParameter( 1, A022 );
//   norm1D2 -> SetParameter( 2, mn2    );
//   norm1D2 -> SetParameter( 3, si2     );
//
//    xproj2 -> Fit("norm1D2");
//
//    double xp1 = norm1D2 -> GetParameter(0);
//    double xp2 = norm1D2 -> GetParameter(1);
//    double xp3 = norm1D2 -> GetParameter(2);
//    double xp4 = norm1D2 -> GetParameter(3);
//
//    double yp1 = norm1D -> GetParameter(0);
//    double yp2 = norm1D -> GetParameter(1);
//    double yp3 = norm1D -> GetParameter(2);
//    double yp4 = norm1D -> GetParameter(3);
//
//    yproj2 -> Fit("norm1D");
//
//    /// Fitting in ROOT
//
//    velaCamStructs::imgCutStruct ics1DCut;
//
//    ics1DCut = bsd.screenImgCutStructs[0];
//
//    int nCutSigma = 3;
//
//
//ics1DCut.x0  = TMath::Nint( xp3 );
//ics1DCut.y0  = TMath::Nint( yp3 );
//ics1DCut.xRad = TMath::Nint( nCutSigma *  TMath::Abs( xp4 ) );
//ics1DCut.yRad = TMath::Nint( nCutSigma *  TMath::Abs( yp4 ) ) ;
//ics1DCut.numPixX = ics1DCut.numX;
//ics1DCut.numPixY = ics1DCut.numY;
//
//    globalVelaCams::calcICSStartAnsStops( ics1DCut );
//
//    std::cout << "ics1DCut.x0 = " << ics1DCut.x0 << std::endl;
//    std::cout << "ics1DCut.y0 = " << ics1DCut.y0 << std::endl;
//    std::cout << "ics1DCut.xRad = " << ics1DCut.xRad << std::endl;
//    std::cout << "ics1DCut.yRad = " << ics1DCut.yRad << std::endl;
//    std::cout << "ics1DCut.xstart = " << ics1DCut.xstart << std::endl;
//    std::cout << "ics1DCut.xstop  = " << ics1DCut.xstop  << std::endl;
//    std::cout << "ics1DCut.ystart = " << ics1DCut.ystart << std::endl;
//    std::cout << "ics1DCut.ystop = " << ics1DCut.ystop << std::endl;
//    std::cout << "ics1DCut.numX = " << ics1DCut.numX << std::endl;
//    std::cout << "ics1DCut.numPixX = " << ics1DCut.numPixX << std::endl;
//    std::cout << "ics1DCut.numPixY = " << ics1DCut.numPixY << std::endl;
//    /// Manually cut, but we need a utility fiunction in globalvela cams etc...
//
//    ///cutData = imageCropper -> cropImage( dataStruct.dataWithBeam[0], ics1DCut );
////
////    ics1DCut.xstart =  141;
////    ics1DCut.xstop  =  669;
////    ics1DCut.ystart =  503;
////    ics1DCut.ystop  =  562;
////    ics1DCut.numX   =  ics1DCut.ystop - ics1DCut.xstart;
////    ics1DCut.numY   =  ics1DCut.ystop - ics1DCut.ystart;
//
//    std::vector< unsigned char > vecToFil( ics1DCut.numY * ics1DCut.numX );
//
//    for( int y = 0; y <  ics1DCut.numY ; ++y )
//    {
//        for( int x = 0; x <  ics1DCut.numX; ++x )
//        {
//            vecToFil[ x + y *  ics1DCut.numX ] = bsd.cutDataWithBeam[ 0 ][ ( ics1DCut.ystart + y ) * ics1DCut.numPixX  + ics1DCut.xstart + x ];
//            //if( x < 2 && y % 100 == 0  )
//                //std::cout << " y = " << y <<  ", x = " << x << ", vecToFil[ x + y *  icsData[ screen ].numX ] = " << (int)vecToFil[ x + y *  icsData[ screen ].numX ] << std::endl;
//        }
//    }
//
//
//    std::cout << "vecToFil.size() = " << vecToFil.size()  << std::endl;
//
//
//    fitTo2DNorm( vecToFil,  ics1DCut );
}




































