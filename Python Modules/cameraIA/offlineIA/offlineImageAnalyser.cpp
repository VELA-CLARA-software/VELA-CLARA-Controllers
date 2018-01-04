#include <fstream>
#include <iterator>
#include <vector>
#include <stdint.h>
#include<iostream>
#include<sstream>
#include <algorithm>
#include <cmath>
#include <chrono>

#include"cameraStructs.h"
#include"offlineImageAnalysis.h"

///--------------------------------IMAGE ANAYLSER FUNCTIONS------------------------------------///
//CONSTRUCTOR
offlineImageAnalyser::offlineImageAnalyser(const bool show_messages,
                                           const bool show_debug_messages
                                           const cameraObject &refData )
: baseObject( &show_messages, &show_debug_messages ),
IO( &show_messages, &show_debug_messages ),
fit( &show_messages, &show_debug_messages ),
edit( &show_messages, &show_debug_messages ),
refToCameraObj = refData
{}
//DESTRUCTOR
offlineImageAnalyser::~offlineImageAnalyser(){}


//read data from file and load into image analyser class in 'imageData'
void offlineImageAnalyser::loadImage(const std::vector<double> originalImage){
    data.rawData = originalImage;
}
//LOAD BACKGROUND IMAGE
void offlineImageAnalyser::loadBackgroundImage(const std::vector<double> originalBkgrndImage){
    data.rawBackgroundData = originalImage;
}
//BEAM ANALYSIS
std::vector<double> offlineImageAnalyser::analyse(){

    //REMOVE BACKGROUND DATA
    if (data.useBkgrnd == true){
        auto start = std::chrono::steady_clock::now();
        data.rawData = edit.subtractImages(data.rawData,
                                           data.rawBackgroundData);
        auto finish = std::chrono::steady_clock::now();
        auto diff  = finish - start;
        message("Finished subtracting background in ",
                (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000);
    }
    else{std::cout<< "No background was removed"<< std::endl;}

    //MAKE THE MASK
    auto start = std::chrono::steady_clock::now();
    if(data.useMaskFromES == true){
        data.mask = edit.makeMask(data,
                                  data.maskXES,data.maskYES,
                                  data.maskRXES,data.maskRYES);
    }
    else{
        data.mask = edit.makeMask(data,
                                  data.x0,data.y0,
                                  data.xRad,data.yRad);
    }
    auto finish = std::chrono::steady_clock::now();
    auto diff  = finish - start;
    message("Finished making mask in ",
            (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000);

    //APPLY THE MASK
    start = std::chrono::steady_clock::now();
    data.rawData = edit.applyMask(data);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    message("Finished applying mask in ",
            (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000);

    //CROP IMAGE
    start = std::chrono::steady_clock::now();
    if(manualCrop==true){
        data = edit.crop(data,manualCropX,manualCropY,manualCropW+manualCropX,manualCropH+manualCropY);
    }
    else{
        imageData = edit.crop(imageData,(imageData.x0-imageData.xRad),(imageData.y0-imageData.yRad),(imageData.x0+imageData.xRad),(imageData.y0+imageData.yRad));
    }
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    message("Finished cropping in ",
            (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000);

    //SET PROJECTIONS
    start = std::chrono::steady_clock::now();
    data.xProjection = edit.getProjection(data,x);
    data.yProjection = edit.getProjection(data,y);
    data.maskXProjection = edit.getProjection(data,maskX);
    data.maskYProjection = edit.getProjection(data,maskY);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    message("Finished getting all projections in ",
            (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000);

    // IMPROVE IMAGE USINF N POINT SCALING METHOD
    start = std::chrono::steady_clock::now();
    data.rawData = edit.nPointScaling(data);
    //have to update projections
    data.xProjection = edit.getProjection(data,x);
    data.yProjection = edit.getProjection(data,y);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    message("Finished N point scaling in ",
            (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000);

    //GENERATE ESTIMATES FOR 1D PROJECTIONS
    start = std::chrono::steady_clock::now();
    std::vector<double>Estim;
    if(useFilterFromES==true){
        Estim = get1DParmaetersForXAndY(filterES);
    }
    else{
        Estim = getBest1DParmaetersForXAndY();
    }
    std::vector<double> x_estimation = {Estim[0],Estim[1],Estim[2],Estim[3]};
    std::vector<double> y_estimation = {Estim[4],Estim[5],Estim[6],Estim[7]};

    //USE ESTIMATES TO RUN GAUSSIAN FITS TO THE PROJECTIONS
    std::vector<double> fitY = fit.fit1DGaussianToProjection(data, y, y_estimation);
    std::vector<double> fitX = fit.fit1DGaussianToProjection(imageData, x, x_estimation);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
   message("Finished first 1D fit in ",
           (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000);

    //FIND R^2 VALUES OF THE FITS (IF >0.4 USE PARAMETERS AS CUTTING VALUES)
    double RRx = fit.rSquaredOf1DProjection(imageData, fitX,x);
    double RRy = fit.rSquaredOf1DProjection(imageData, fitY,y);
    std::cout<<"R squared values of x and y fits respectively:"<< std::endl;
    std::cout<<RRx<<" "<<RRy<<std::endl;
    double cMuX,cMuY,cSigmaX,cSigmaY;
    // if fits are not good used FWHM estimates
    if(useRRThresholdFromES==true){
        if(RRx<RRThresholdES){cMuX=Estim[2];cSigmaX=Estim[3];}
        else{cMuX=fitX[2];cSigmaX=fitX[3];}

        if(RRy<RRThresholdES){cMuY=Estim[6];cSigmaY=Estim[7];}
        else{cMuY=fitY[2];cSigmaY=fitY[3];}
    }
    else{//default is 0.4
        if(RRx<0.4){cMuX=Estim[2];cSigmaX=Estim[3];}
        else{cMuX=fitX[2];cSigmaX=fitX[3];}

        if(RRy<0.4){cMuY=Estim[6];cSigmaY=Estim[7];}
        else{cMuY=fitY[2];cSigmaY=fitY[3];}
    }


    //CROP USING MU AND SIGMA
    int xC,yC,wC,hC;
    if(useSigmaCutFromES==true){
        xC = (int)round(cMuX-(sigmaCutES*cSigmaX));
        yC = (int)round(cMuY-(sigmaCutES*cSigmaY));
        wC = (int)round(cMuX+(sigmaCutES*cSigmaX));
        hC = (int)round(cMuY+(sigmaCutES*cSigmaY));
    }
    else{//default is three sigma
        xC = (int)round(cMuX-(3*cSigmaX));
        yC = (int)round(cMuY-(3*cSigmaY));
        wC = (int)round(cMuX+(3*cSigmaX));
        hC = (int)round(cMuY+(3*cSigmaY));
    }

    //CROP IMAGE DOWN FURTHER
    if(manualCrop==false){
        start = std::chrono::steady_clock::now();
        imageData = edit.crop(imageData,xC,yC,wC,hC);
        finish = std::chrono::steady_clock::now();
        diff  = finish - start;
        std::cout<<"Finished crop from fit estimates in "<< (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<< std::endl;
    }

    //UPDATE PROJECTIONS
    imageData.xProjection = IO.getProjection(imageData,x);
    imageData.yProjection = IO.getProjection(imageData,y);

    //RECALCULATE ESTIMATES AND THEN FITS
    std::vector<double> Estim_ReCalc;
    if(useFilterFromES==true){
        Estim_ReCalc = get1DParmaetersForXAndY(filterES);
    }
    else{

        Estim_ReCalc = getBest1DParmaetersForXAndY();
    }
    std::vector<double> x_estimation_ReCalc = {Estim_ReCalc[0],Estim_ReCalc[1],Estim_ReCalc[2],Estim_ReCalc[3]};
    std::vector<double> y_estimation_ReCalc = {Estim_ReCalc[4],Estim_ReCalc[5],Estim_ReCalc[6],Estim_ReCalc[7]};


    //OVERIDING OLD FIT PARAMETERS
    std::vector<double> refitY = fit.fit1DGaussianToProjection(imageData, y, y_estimation_ReCalc);
    std::vector<double> refitX = fit.fit1DGaussianToProjection(imageData, x, x_estimation_ReCalc);
    std::cout<<"Finished re-calc of estimates and 1D re-fits"<< std::endl;


    //USING 1D FIT PARAMATERS FIT BVN TO IMAGE
    start = std::chrono::steady_clock::now();
    ///Curently not using this so just outputs 1s
    std::vector<double> fitImageBVN = {1,1,1,1,1,1,1};
    //std::vector<double> fitImageBVN = fit.fitBVN(imageData,refitX,refitY);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    message("Finished BVN fit in ",
            (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000);

    //CORRECT CENTROID POSITION (from the cropping)
    std::vector<double> corrections = edit.correctBeamPosition(fitImageBVN[2],fitImageBVN[3]);
    fitImageBVN[2] = corrections[0];
    fitImageBVN[3] = corrections[1];
    std::cout<<"Finished BVN corrections"<< std::endl;


    //DIRECTY FIT TO DATA WITHOUT ESTIMATES
    start = std::chrono::steady_clock::now();
    std::vector<double> fitImageCov= {1,1,1,1,1};
    if(useDirectCutLevelFromES==true){ fitImageCov = fit.covarianceValues(imageData,DirectCutLevelES);}
    else{ fitImageCov = fit.covarianceValues(imageData,0);}
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    message("Finished direct calc of parameters in ",
            (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000);

    std::vector<double> correctionsCov = edit.correctBeamPosition(fitImageCov[0],fitImageCov[1]);
    fitImageCov[0] = correctionsCov[0];
    fitImageCov[1] = correctionsCov[1];
    std::cout<<"Finished corrections"<< std::endl;
    //clear up loose ends
    savedCroppedX = edit.croppedX;
    savedCroppedY = edit.croppedY;
    edit.croppedX=0;
    edit.croppedY=0;
    //imageData.clear();



    //OUTPUT IMAGE PARAMETERS in one string (BVN FIT,COVARIANCE FIT, 1D ESTIMATES FOR X, 1D ESTIMATES FOR Y)
    std::vector<double> output; //

    output.insert(output.end(),fitImageBVN.begin()+2,fitImageBVN.end());
    output.insert(output.end(),fitImageCov.begin(),fitImageCov.end());
    output.insert(output.end(),refitX.begin()+2,refitX.end());
    output.insert(output.end(),refitY.begin()+2,refitY.end());

    return output;

}
//GET THE BEST 1D PARAMETERS BY APPLYING DIFFERENT FILTERS (RETURNS FWHM ESTIMATES)
std::vector<double> offlineImageAnalyser::getBest1DParmaetersForXAndY(){

    std::vector<double> wx(4,0),wy(4,0);
    std::vector<std::vector<double>> FWHMx(4,{0,0,0,0}),FWHMy(4,{0,0,0,0});

    std::vector<double> noFiltX = edit.applyFilter(data,1,x);
    std::vector<double> momx =  fit.get1DEstimates(noFiltX,moments);
    FWHMx[0]= fit.get1DEstimates(noFiltX,FWHM);
    wx[0] =  fit.compare1DEstimates(momx[2],momx[3],FWHMx[0][2],FWHMx[0][3]);


    std::vector<double> filt5x =  edit.applyFilter(data,5,x);
    std::vector<double> mom5x =  fit.get1DEstimates(filt5x,moments);
    FWHMx[1] =  fit.get1DEstimates(filt5x,FWHM);
    wx[1]=  fit.compare1DEstimates(mom5x[2],mom5x[3],FWHMx[1][2],FWHMx[1][3]);

    std::vector<double> filt10x = edit.applyFilter(data,10,x);
    std::vector<double> mom10x =  fit.get1DEstimates(filt10x,moments);
    FWHMx[2] =  fit.get1DEstimates(filt10x,FWHM);
    wx[2] =  fit.compare1DEstimates(mom10x[2],mom10x[3],FWHMx[2][2],FWHMx[2][3]);

    std::vector<double> filt20x = edit.applyFilter(data,20,x);
    std::vector<double> mom20x =  fit.get1DEstimates(filt20x,moments);
    FWHMx[3] =  fit.get1DEstimates(filt20x,FWHM);
    wx[3] =  fit.compare1DEstimates(mom20x[2],mom20x[3],FWHMx[3][2],FWHMx[3][3]);


    //Apply different filters and decid which is best FOR Y
    std::vector<double> noFiltY = edit.applyFilter(data,1,y);
    std::vector<double> momy =  fit.get1DEstimates(noFiltY,moments);
    FWHMy[0] =  fit.get1DEstimates(noFiltY,FWHM);
    wy[0] =  fit.compare1DEstimates(momy[2],momy[3],FWHMy[0][2],FWHMy[0][3]);

    std::vector<double> filt5y = edit.applyFilter(data,5,y);
    std::vector<double> mom5y =  fit.get1DEstimates(filt5y,moments);
    FWHMy[1] =  fit.get1DEstimates(filt5y,FWHM);
    wy[1] =  fit.compare1DEstimates(mom5y[2],mom5y[3],FWHMy[1][2],FWHMy[1][3]);

    std::vector<double> filt10y = edit.applyFilter(data,10,y);
    std::vector<double> mom10y =  fit.get1DEstimates(filt10y,moments);
    FWHMy[2] =  fit.get1DEstimates(filt10y,FWHM);
    wy[2] =  fit.compare1DEstimates(mom10y[2],mom10y[3],FWHMy[2][2],FWHMy[2][3]);

    std::vector<double> filt20y = edit.applyFilter(data,20,y);
    std::vector<double> mom20y =  fit.get1DEstimates(filt20y,moments);
    FWHMy[3] =  fit.get1DEstimates(filt20y,FWHM);
    wy[3] =  fit.compare1DEstimates(mom20y[2],mom20y[3],FWHMy[3][2],FWHMy[3][3]);

    //Find min w for x and y
    std::vector<double>::iterator wMinX = std::min_element(wx.begin(),wx.end());
    std::vector<double>::iterator wMinY = std::min_element(wy.begin(),wy.end());


    //Get the estimates that correspond to those minimum w values and output the corresponding estimates
    std::vector<double> output; //(xA,xB,xMu,xSigma,yA,yB,yMu,ySigma)

    output.insert(output.end(),FWHMx[std::distance(wx.begin(),wMinX)].begin(),FWHMx[std::distance(wx.begin(),wMinX)].end());

    output.insert(output.end(),FWHMy[std::distance(wy.begin(),wMinY)].begin(),FWHMy[std::distance(wy.begin(),wMinY)].end());

    return output;

}
//GET THE ESTIMATES USING A SPECIFIC FILTER
std::vector<double> offlineImageAnalyser::get1DParmaetersForXAndY(const int& filter){

    std::vector<double> FWHMx={0,0,0,0},FWHMy={0,0,0,0};

    std::vector<double> FiltX = edit.applyFilter(data,filter,x);

    FWHMx= fit.get1DEstimates(FiltX,FWHM);

    std::vector<double> FiltY = edit.applyFilter(data,filter,y);

    FWHMy =  fit.get1DEstimates(FiltY,FWHM);

    //Get the estimates that correspond to those minimum w values and output the corresponding estimates
    std::vector<double> output; //(xA,xB,xMu,xSigma,yA,yB,yMu,ySigma)
    output.insert(output.end(),FWHMx.begin(),FWHMx.end());
    output.insert(output.end(),FWHMy.begin(),FWHMy.end());

    return output;

}

void offlineImageAnalyser::crop(const int& x,const int& y,const int& w,const int& h){
    //CROP IMAGE
    data = edit.crop(data,x,y,w,h);
    std::cout<<"Finished cropping"<<std::endl;

}
//Functions to set ES bools
void offlineImageAnalyser::useBackground(const bool& tf){
    bkgrnd=tf;
}
void offlineImageAnalyser::useManualCrop(const bool& tf){
    manualCrop=tf;
}
void offlineImageAnalyser::useESMask(const bool& tf){
    useMaskFromES=tf;
}
void offlineImageAnalyser::useESPixToMm(const bool& tf){
    usePixToMmFromES=tf;
}
void offlineImageAnalyser::useESRRThreshold(const bool& tf){
    useRRThresholdFromES=tf;
}
void offlineImageAnalyser::useESSigmaCut(const bool& tf){
    useSigmaCutFromES=tf;
}
void offlineImageAnalyser::useESFilter(const bool& tf){
    useFilterFromES=tf;
}
void offlineImageAnalyser::useESDirectCut(const bool& tf){
    useDirectCutLevelFromES=tf;
}
// Functions to set ES data
void offlineImageAnalyser::setManualCrop(const int& x,const int& y,const int& w,const int& h){
    manualCropX=x;
    manualCropY=y;
    manualCropW=w;
    manualCropH=h;
}
void offlineImageAnalyser::setESMask(const int& x,const int& y,const int& rx,const int& ry){
    maskXES=x;
    maskYES=y;
    maskRXES=rx;
    maskRYES=ry;
}
void offlineImageAnalyser::setESPixToMm(const double& ptm){
    pixToMmES=ptm;
}
void offlineImageAnalyser::setESRRThreshold(const double& rrt){
    RRThresholdES=rrt;
}
void offlineImageAnalyser::setESSigmaCut(const double& sc){
    sigmaCutES=sc;
}
void offlineImageAnalyser::setESFilter(const int& f){
    filterES=f;
}
void offlineImageAnalyser::setESDirectCut(const double& dc){
    DirectCutLevelES=dc;
}
double offlineImageAnalyser::getPTMRatio(){
    return imageData.pixToMM;
}

