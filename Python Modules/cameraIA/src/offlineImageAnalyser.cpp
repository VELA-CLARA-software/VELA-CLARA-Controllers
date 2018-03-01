#include <fstream>
#include <iterator>
#include <vector>
#include <stdint.h>
#include<iostream>
#include<sstream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <thread>
#include<mutex>
#include"cameraStructs.h"
#include"offlineImageAnalyser.h"


std::mutex mu;
///--------------------------------IMAGE ANAYLSER FUNCTIONS------------------------------------///
//CONSTRUCTOR
offlineImageAnalyser::offlineImageAnalyser(const bool show_messages,
                                           const bool show_debug_messages,
                                           cameraIAInterface* CI)
: //baseObject( &show_messages, &show_debug_messages ),
fit( &show_messages, &show_debug_messages ),
edit( &show_messages, &show_debug_messages )//,
//CoIA()
{
cameraInterface=CI;
}
//DESTRUCTOR
offlineImageAnalyser::~offlineImageAnalyser(){}


//read data from file and load into image analyser class in 'imageData'
void offlineImageAnalyser::loadImage(const std::vector<double> &originalImage,
                                     const std::string &name,
                                     int hieght, int width){
    CoIA.clear();
    CoIA.imageHeight = hieght;
    CoIA.imageWidth = width;
    CoIA.dataSize = hieght*width;
    CoIA.imageName = name;
    CoIA.x0 = width/2;
    CoIA.y0 = hieght/2;
    CoIA.xRad = width/2;
    CoIA.yRad = hieght/2;
    for(auto && it : (*cameraInterface).allCamData)
    {
        if (name.find(it.second.name) != std::string::npos)
        {
            std::cout << "Found  and using config parameters for camera " << it.second.name <<std::endl;

            CoIA.pixToMM = it.second.IA.pix2mm;
            //overwrite default values
            CoIA.x0 = it.second.IA.maskX;
            CoIA.y0 = it.second.IA.maskY;
            CoIA.xRad = it.second.IA.maskXRad;
            CoIA.yRad = it.second.IA.maskYRad;
        }
    }
//#std::vector<double> dummy(CoIA.dataSize);
 //   CoIA.mask = dummy;
    CoIA.mask.clear();
    CoIA.rawData.clear();
    for(auto i=0;i!=originalImage.size();++i){
        CoIA.rawData.push_back(originalImage[i]);
        CoIA.mask.push_back(originalImage[i]);
    }
    //set position of pixels
    for(auto i=CoIA.imageHeight-1;i>=0;i--){
        for(auto j=0;j<CoIA.imageWidth;j++){
            CoIA.xPos.push_back((double)j);
            CoIA.yPos.push_back((double)i);
        }
    }
}
//LOAD BACKGROUND IMAGE
void offlineImageAnalyser::loadBackgroundImage(const std::vector<double> &originalBkgrndImage, const std::string &name){
    CoIA.rawBackgroundData.clear();
    CoIA.bkgrndName = name;
    for(auto i=0;i!=originalBkgrndImage.size();++i){
        CoIA.rawBackgroundData.push_back(originalBkgrndImage[i]);
    }
}
//WRITE DATA TO CSV
void offlineImageAnalyser::writeData(const std::string &fileName){
      std::ifstream checkFile(fileName);
      if(checkFile.good()){
        std::cout<< "File already exits."<< std::endl;
      }
      else{
         std::cout<< "Creating a new file."<< std::endl;
         std::ofstream createFile(fileName);
         createFile.close();
      }
      std::fstream myfile(fileName,std::ios::in|std::ios::out);
      myfile.seekg(0, std::ios::end); // put the "cursor" at the end of the file
      int length = myfile.tellg();
      if ( length == 0){
            std::cout<< "Writing data to new file..."<< std::endl;
            myfile << "ANALYSIS DATA\n";
            myfile << "Expert Settings:\n";
            myfile << "Mask,,,,R^2 Threshold,Rolling Average,Pecentage Cut of Lowest Pixels\n";
            myfile << "X,Y,X Radius,Y Radius\n";
            myfile << CoIA.maskXES <<","<<CoIA.maskYES <<","<< CoIA.maskRXES <<","<< CoIA.maskRYES <<","<< CoIA.RRThresholdES <<","<< CoIA.filterES <<","<< CoIA.DirectCutLevelES <<"\n";
            myfile<< "BVN, , , , , , , , , ,MLE, , , , , , , , , , \n";
            myfile<< "X,err,Y,err,Sigma X,err, Sigma Y,err,Covariance XY,err,X,err,Y,err,Sigma X,err, Sigma Y,err,Covariance XY,err,";
            myfile<< " ,pix2MM,,Image Name,Background Image Name\n";
      }
      else {
          std::cout<< "Writing data to a file that already exits..."<< std::endl;
      }
      myfile << CoIA.xBVN <<",";
      myfile << CoIA.xBVNerr <<",";
      myfile << CoIA.yBVN <<",";
      myfile << CoIA.yBVNerr <<",";
      myfile << CoIA.sxBVN <<",";
      myfile << CoIA.sxBVNerr <<",";
      myfile << CoIA.syBVN <<",";
      myfile << CoIA.syBVNerr <<",";
      myfile << CoIA.cxyBVN <<",";
      myfile << CoIA.cxyBVNerr <<",";
      myfile << CoIA.xMLE <<",";
      myfile << CoIA.xMLEerr <<",";
      myfile << CoIA.yMLE <<",";
      myfile << CoIA.yMLEerr<<",";
      myfile << CoIA.sxMLE <<",";
      myfile << CoIA.sxMLEerr <<",";
      myfile << CoIA.syMLE <<",";
      myfile << CoIA.syMLEerr <<",";
      myfile << CoIA.cxyMLE <<",";
      myfile << CoIA.cxyMLEerr <<",,";
      myfile << CoIA.pixToMM << ",,";
      myfile << CoIA.imageName <<",";
      myfile << CoIA.bkgrndName <<"\n";

      myfile.close();
      std::cout<< "Done."<< std::endl;
}

//BEAM ANALYSIS
/*
void offlineImageAnalyser::analyse(){

    //REMOVE BACKGROUND DATA
    if (CoIA.useBkgrnd == true){
        auto start = std::chrono::steady_clock::now();
        CoIA.rawData = edit.subtractBackground(CoIA);
        auto finish = std::chrono::steady_clock::now();
        auto diff  = finish - start;
        std::cout<<"Finished subtracting background in "<<(double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<<std::endl;
    }
    else{std::cout<< "No background was removed"<< std::endl;}

    //MAKE THE MASK
    int x,y,xR,yR;
    auto start = std::chrono::steady_clock::now();
    if(CoIA.useMaskFromES == true){
        x = CoIA.maskXES;
        y = CoIA.maskYES;
        xR = CoIA.maskRXES;
        yR = CoIA.maskRYES;
    }
    else{
        x = CoIA.x0;
        y = CoIA.y0;
        xR = CoIA.xRad;
        yR = CoIA.yRad;
    }
    CoIA.mask = edit.makeMask(CoIA,x,y,xR,yR);
    auto finish = std::chrono::steady_clock::now();
    auto diff  = finish - start;
    std::cout<<"Finished making mask in "<<(double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<<std::endl;

    //APPLY THE MASK
    start = std::chrono::steady_clock::now();
    CoIA.rawData = edit.applyMask(CoIA);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    std::cout<<"Finished applying mask in "<<(double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<<std::endl;

    //CROP IMAGE
    start = std::chrono::steady_clock::now();
    CoIA = edit.crop(CoIA,(x-xR),(y-yR), (x+xR), (y+yR));
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    std::cout<<"Finished cropping in "<<(double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<<std::endl;

    //SET PROJECTIONS
    start = std::chrono::steady_clock::now();
    CoIA.xProjection = edit.getProjection(CoIA,projAxis::x);
    CoIA.yProjection = edit.getProjection(CoIA,projAxis::y);
    CoIA.maskXProjection = edit.getProjection(CoIA,projAxis::maskX);
    CoIA.maskYProjection = edit.getProjection(CoIA,projAxis::maskY);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    std::cout<<"Finished getting all projections in "<<(double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<<std::endl;

    // IMPROVE IMAGE USING N POINT SCALING METHOD
    start = std::chrono::steady_clock::now();
    CoIA.rawData = edit.nPointScaling(CoIA);
    std::cout<< "image after  N point scalling: " <<CoIA.rawData.size()<<std::endl;
    //have to update projections
    CoIA.xProjection = edit.getProjection(CoIA,projAxis::x);
    CoIA.yProjection = edit.getProjection(CoIA,projAxis::y);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    std::cout<<"Finished N point scaling in "<<(double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<<std::endl;


    //GENERATE ESTIMATES FOR 1D PROJECTIONS
    start = std::chrono::steady_clock::now();
    std::vector<double>Estim;
    if(CoIA.useFilterFromES==true){
        Estim = get1DParmaetersForXAndY(CoIA.filterES);
    }
    else{
        Estim = getBest1DParmaetersForXAndY();
    }
    std::vector<double> x_estimation = {Estim[0],Estim[1],Estim[2],Estim[3]};
    std::vector<double> y_estimation = {Estim[4],Estim[5],Estim[6],Estim[7]};

    //USE ESTIMATES TO RUN GAUSSIAN FITS TO THE PROJECTIONS
    std::vector<double> fitY = fit.fit1DGaussianToProjection(CoIA,
                                                             projAxis::y,
                                                             y_estimation);
    std::vector<double> fitX = fit.fit1DGaussianToProjection(CoIA,
                                                             projAxis::x,
                                                             x_estimation);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    std::cout<<"Finished first 1D fit in "<<
           (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<<std::endl;

    //FIND R^2 VALUES OF THE FITS (IF >0.4 USE PARAMETERS AS CUTTING VALUES)
    double RRx = fit.rSquaredOf1DProjection(CoIA, fitX,projAxis::x);
    double RRy = fit.rSquaredOf1DProjection(CoIA, fitY,projAxis::y);
    std::cout<<"R squared values of x and y fits respectively:"<< std::endl;
    std::cout<<RRx<<" "<<RRy<<std::endl;
    double cMuX,cMuY,cSigmaX,cSigmaY;
    // if fits are not good used FWHM estimates
    if(CoIA.useRRThresholdFromES==true){
        if(RRx<CoIA.RRThresholdES){cMuX=Estim[2];cSigmaX=Estim[3];}
        else{cMuX=fitX[2];cSigmaX=fitX[3];}

        if(RRy<CoIA.RRThresholdES){cMuY=Estim[6];cSigmaY=Estim[7];}
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
    if(CoIA.useSigmaCutFromES==true){
        xC = (int)round(cMuX-(CoIA.sigmaCutES*cSigmaX));
        yC = (int)round(cMuY-(CoIA.sigmaCutES*cSigmaY));
        wC = (int)round(cMuX+(CoIA.sigmaCutES*cSigmaX));
        hC = (int)round(cMuY+(CoIA.sigmaCutES*cSigmaY));
    }
    else{//default is three sigma
        xC = (int)round(cMuX-(3*cSigmaX));
        yC = (int)round(cMuY-(3*cSigmaY));
        wC = (int)round(cMuX+(3*cSigmaX));
        hC = (int)round(cMuY+(3*cSigmaY));
    }

    //CROP IMAGE DOWN FURTHER

    start = std::chrono::steady_clock::now();
    CoIA = edit.crop(CoIA,xC,yC,wC,hC);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    std::cout<<"Finished crop from fit estimates in "<< (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<< std::endl;

    //UPDATE PROJECTIONS
    CoIA.xProjection = edit.getProjection(CoIA,projAxis::x);
    CoIA.yProjection = edit.getProjection(CoIA,projAxis::y);

    //RECALCULATE ESTIMATES AND THEN FITS
    std::vector<double> Estim_ReCalc;
    if(CoIA.useFilterFromES==true){
        Estim_ReCalc = get1DParmaetersForXAndY(CoIA.filterES);
    }
    else{

        Estim_ReCalc = getBest1DParmaetersForXAndY();
    }
    std::vector<double> x_estimation_ReCalc = {Estim_ReCalc[0],Estim_ReCalc[1],Estim_ReCalc[2],Estim_ReCalc[3]};
    std::vector<double> y_estimation_ReCalc = {Estim_ReCalc[4],Estim_ReCalc[5],Estim_ReCalc[6],Estim_ReCalc[7]};


    //OVERIDING OLD FIT PARAMETERS
    std::vector<double> refitY = fit.fit1DGaussianToProjection(CoIA, projAxis::y, y_estimation_ReCalc);
    std::vector<double> refitX = fit.fit1DGaussianToProjection(CoIA, projAxis::x, x_estimation_ReCalc);
    std::cout<<"Finished re-calc of estimates and 1D re-fits"<< std::endl;


    //USING 1D FIT PARAMATERS FIT BVN TO IMAGE
    start = std::chrono::steady_clock::now();
    ///Curently not using this so just outputs 1s
    std::vector<double> fitImageBVN = {1,1,1,1,1,1,1};
    fitImageBVN = fit.fitBVN(CoIA,refitX,refitY);
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    std::cout<<"Finished BVN fit in "<<(double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<<std::endl;

    //CORRECT CENTROID POSITION (from the cropping)
    std::vector<double> corrections = edit.correctBeamPosition(fitImageBVN[2],fitImageBVN[3]);
    fitImageBVN[2] = corrections[0];
    fitImageBVN[3] = corrections[1];
    std::cout<<"Finished BVN corrections"<< std::endl;


    //DIRECTY FIT TO DATA WITHOUT ESTIMATES
    start = std::chrono::steady_clock::now();
    std::vector<double> fitImageCov= {1,1,1,1,1};
    if(CoIA.useDirectCutLevelFromES==true){ fitImageCov = fit.covarianceValues(CoIA,CoIA.DirectCutLevelES);}
    else{ fitImageCov = fit.covarianceValues(CoIA,0);}
    finish = std::chrono::steady_clock::now();
    diff  = finish - start;
    std::cout<<"Finished direct calc of parameters in "<<(double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000<<std::endl;

    std::vector<double> correctionsCov = edit.correctBeamPosition(fitImageCov[0],fitImageCov[1]);
    fitImageCov[0] = correctionsCov[0];
    fitImageCov[1] = correctionsCov[1];
    std::cout<<"Finished corrections"<< std::endl;
    //clear up loose ends
    CoIA.savedCroppedX = edit.croppedX;
    CoIA.savedCroppedY = edit.croppedY;
    edit.croppedX=0;
    edit.croppedY=0;
    //CoIA.clear();

    //OUTPUT IMAGE PARAMETERS
    CoIA.xBVN = fitImageBVN[2];
    CoIA.yBVN = fitImageBVN[3];
    CoIA.sxBVN = sqrt(fitImageBVN[4]);
    CoIA.syBVN = sqrt(fitImageBVN[5]);
    CoIA.cxyBVN = fitImageBVN[6];
    CoIA.xMLE = fitImageCov[0];
    CoIA.yMLE = fitImageCov[1];
    CoIA.sxMLE = sqrt(fitImageCov[2]);
    CoIA.syMLE = sqrt(fitImageCov[3]);
    CoIA.cxyMLE = fitImageCov[4];



}*/
bool offlineImageAnalyser::killLongRunningThread(const double& timeOut){
    bool ans =false;
    stopTime = std::chrono::steady_clock::now();
    auto diff  = stopTime - startTime;
    double diffD = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()/1000000000;
    if(diffD>timeOut)
    {
        std::cout<<"This has gone on too long!!"<< std::endl;
        delete threadedAnalysis;///THIS TERMINATES THE WHOLE CODE!!!!!!!!!!!!
        ans=true;
    }
    return ans;
}
void offlineImageAnalyser::analyse(){
    //need to make sure your python is running while this thread is active
    startTime = std::chrono::steady_clock::now();
   threadedAnalysis = new std::thread (&offlineImageAnalyser::staticAnalyse,
                                                      this);
    stopTime = std::chrono::steady_clock::now();

}
bool offlineImageAnalyser::staticAnalyse(){
    this->analysing=true;

    //REMOVE BACKGROUND DATA
    if (this->CoIA.useBkgrnd == true){
        this->CoIA.rawData = this->edit.subtractBackground(this->CoIA);
    }
    else{
        std::cout<< "No background was removed"<< std::endl;
    }

    //MAKE THE MASK
    int x,y,xR,yR;
    if(this->CoIA.useMaskFromES == true){
        x = this->CoIA.maskXES;
        y = this->CoIA.maskYES;
        xR = this->CoIA.maskRXES;
        yR = this->CoIA.maskRYES;
    }
    else{
        x = this->CoIA.x0;
        y = this->CoIA.y0;
        xR = this->CoIA.xRad;
        yR = this->CoIA.yRad;
    }
    this->CoIA.mask = this->edit.makeMask(this->CoIA,x,y,xR,yR);

    //APPLY THE MASK
    this->CoIA.rawData = this->edit.applyMask(this->CoIA);

    //CROP IMAGE
    this->CoIA = this->edit.crop(this->CoIA,(x-xR),(y-yR), (x+xR), (y+yR));

    //SET PROJECTIONS
    this->CoIA.xProjection =this-> edit.getProjection(this->CoIA,projAxis::x);
    this->CoIA.yProjection = this->edit.getProjection(this->CoIA,projAxis::y);
    this->CoIA.maskXProjection = this->edit.getProjection(this->CoIA,projAxis::maskX);
    this->CoIA.maskYProjection = this->edit.getProjection(this->CoIA,projAxis::maskY);

    // IMPROVE IMAGE USING N POINT SCALING METHOD
    this->CoIA.rawData = this->edit.nPointScaling(this->CoIA);
    //have to update projections
    this->CoIA.xProjection = this->edit.getProjection(this->CoIA,projAxis::x);
    this->CoIA.yProjection = this->edit.getProjection(this->CoIA,projAxis::y);

    //GENERATE ESTIMATES FOR 1D PROJECTIONS
    std::vector<double>Estim;
    if(this->CoIA.useFilterFromES==true){
        Estim = this->get1DParmaetersForXAndY(this->CoIA.filterES);
    }
    else{
        Estim = this->getBest1DParmaetersForXAndY();
    }
    std::vector<double> x_estimation = {Estim[0],Estim[1],Estim[2],Estim[3]};
    std::vector<double> y_estimation = {Estim[4],Estim[5],Estim[6],Estim[7]};

    //USE ESTIMATES TO RUN GAUSSIAN FITS TO THE PROJECTIONS
    std::vector<double> fitY = this->fit.fit1DGaussianToProjection(this->CoIA,
                                                             projAxis::y,
                                                             y_estimation);
    std::vector<double> fitX = this->fit.fit1DGaussianToProjection(this->CoIA,
                                                             projAxis::x,
                                                             x_estimation);

    //FIND R^2 VALUES OF THE FITS (IF >0.4 USE PARAMETERS AS CUTTING VALUES)
    double RRx = this->fit.rSquaredOf1DProjection(this->CoIA, fitX,projAxis::x);
    double RRy = this->fit.rSquaredOf1DProjection(this->CoIA, fitY,projAxis::y);
    std::cout<<"R squared values of x and y fits respectively:"<< std::endl;
    std::cout<<RRx<<" "<<RRy<<std::endl;
    double cMuX,cMuY,cSigmaX,cSigmaY;
    // if fits are not good used FWHM estimates
    if(this->CoIA.useRRThresholdFromES==true){
        if(RRx<this->CoIA.RRThresholdES){cMuX=Estim[2];cSigmaX=Estim[3];}
        else{cMuX=fitX[2];cSigmaX=fitX[3];}

        if(RRy<this->CoIA.RRThresholdES){cMuY=Estim[6];cSigmaY=Estim[7];}
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
    if(this->CoIA.useSigmaCutFromES==true){
        xC = (int)round(cMuX-(this->CoIA.sigmaCutES*cSigmaX));
        yC = (int)round(cMuY-(this->CoIA.sigmaCutES*cSigmaY));
        wC = (int)round(cMuX+(this->CoIA.sigmaCutES*cSigmaX));
        hC = (int)round(cMuY+(this->CoIA.sigmaCutES*cSigmaY));
    }
    else{//default is three sigma
        xC = (int)round(cMuX-(3*cSigmaX));
        yC = (int)round(cMuY-(3*cSigmaY));
        wC = (int)round(cMuX+(3*cSigmaX));
        hC = (int)round(cMuY+(3*cSigmaY));
    }

    //CROP IMAGE DOWN FURTHER

    this->CoIA = this->edit.crop(this->CoIA,xC,yC,wC,hC);

    //UPDATE PROJECTIONS
    this->CoIA.xProjection = this->edit.getProjection(this->CoIA,projAxis::x);
    this->CoIA.yProjection = this->edit.getProjection(this->CoIA,projAxis::y);

    //RECALCULATE ESTIMATES AND THEN FITS
    std::vector<double> Estim_ReCalc;
    if(this->CoIA.useFilterFromES==true){
        Estim_ReCalc = this->get1DParmaetersForXAndY(this->CoIA.filterES);
    }
    else{

        Estim_ReCalc = this->getBest1DParmaetersForXAndY();
    }
    std::vector<double> x_estimation_ReCalc = {Estim_ReCalc[0],Estim_ReCalc[1],Estim_ReCalc[2],Estim_ReCalc[3]};
    std::vector<double> y_estimation_ReCalc = {Estim_ReCalc[4],Estim_ReCalc[5],Estim_ReCalc[6],Estim_ReCalc[7]};

    //OVERIDING OLD FIT PARAMETERS
    std::vector<double> refitY = this->fit.fit1DGaussianToProjection(this->CoIA, projAxis::y, y_estimation_ReCalc);
    std::vector<double> refitX = this->fit.fit1DGaussianToProjection(this->CoIA, projAxis::x, x_estimation_ReCalc);
    std::cout<<"Finished re-calc of estimates and 1D re-fits"<< std::endl;


    //USING 1D FIT PARAMATERS FIT BVN TO IMAGE
    std::vector<double> fitImageBVN = {1,1,1,1,1,1,1};
    fitImageBVN = this->fit.fitBVN(this->CoIA,refitX,refitY);

    //CORRECT CENTROID POSITION (from the cropping)
    std::vector<double> corrections = this->edit.correctBeamPosition(fitImageBVN[2],fitImageBVN[3]);
    fitImageBVN[2] = corrections[0];
    fitImageBVN[3] = corrections[1];
    std::cout<<"Finished BVN corrections"<< std::endl;


    //DIRECTY FIT TO DATA WITHOUT ESTIMATES
    std::vector<double> fitImageCov= {1,1,1,1,1};
    if(this->CoIA.useDirectCutLevelFromES==true){ fitImageCov = this->fit.covarianceValues(this->CoIA,this->CoIA.DirectCutLevelES, this->CoIA.pixelValueError);}
    else{ fitImageCov = this->fit.covarianceValues(this->CoIA,0, this->CoIA.pixelValueError);}

    std::vector<double> correctionsCov = this->edit.correctBeamPosition(fitImageCov[0],fitImageCov[1]);
    fitImageCov[0] = correctionsCov[0];
    fitImageCov[1] = correctionsCov[1];
    std::cout<<"Finished corrections"<< std::endl;
    //clear up loose ends
    this->CoIA.savedCroppedX = this->edit.croppedX;
    this->CoIA.savedCroppedY = this->edit.croppedY;
    this->edit.croppedX=0;
    this->edit.croppedY=0;
    //CoIA.clear();

    //OUTPUT IMAGE PARAMETERS
    this->CoIA.xBVN = fitImageBVN[2];
    this->CoIA.yBVN = fitImageBVN[3];
    this->CoIA.sxBVN = sqrt(fitImageBVN[4]);
    this->CoIA.syBVN = sqrt(fitImageBVN[5]);
    this->CoIA.cxyBVN = fitImageBVN[6];
    this->CoIA.xMLE = fitImageCov[0];
    this->CoIA.yMLE = fitImageCov[1];
    this->CoIA.sxMLE = sqrt(fitImageCov[2]);
    this->CoIA.syMLE = sqrt(fitImageCov[3]);
    this->CoIA.cxyMLE = fitImageCov[4];

    this->CoIA.xBVNerr = fitImageBVN[9];
    this->CoIA.yBVNerr = fitImageBVN[10];
    this->CoIA.sxBVNerr = fitImageBVN[11]/(2*fitImageBVN[4]);//Error propagation calculation
    this->CoIA.syBVNerr = fitImageBVN[12]/(2*fitImageBVN[5]);
    this->CoIA.cxyBVNerr = fitImageBVN[13];
    this->CoIA.xMLEerr = fitImageCov[5];
    this->CoIA.yMLEerr = fitImageCov[6];
    this->CoIA.sxMLEerr = fitImageCov[7]/(2*fitImageCov[2]);
    this->CoIA.syMLEerr = fitImageCov[8]/(2*fitImageCov[3]);
    this->CoIA.cxyMLEerr = fitImageCov[9];
    this->analysing=false;

    return true;

}
//GET THE BEST 1D PARAMETERS BY APPLYING DIFFERENT FILTERS (RETURNS FWHM ESTIMATES)
std::vector<double> offlineImageAnalyser::getBest1DParmaetersForXAndY(){

    std::vector<double> wx(4,0),wy(4,0);
    std::vector<std::vector<double>> FWHMx(4,{0,0,0,0}),FWHMy(4,{0,0,0,0});

    std::vector<double> noFiltX = edit.applyFilter(CoIA,1,projAxis::x);
    std::vector<double> momx =  fit.get1DEstimates(noFiltX,EstMethod::moments);
    FWHMx[0]= fit.get1DEstimates(noFiltX,EstMethod::FWHM);
    wx[0] =  fit.compare1DEstimates(momx[2],momx[3],FWHMx[0][2],FWHMx[0][3]);


    std::vector<double> filt5x =  edit.applyFilter(CoIA,5,projAxis::x);
    std::vector<double> mom5x =  fit.get1DEstimates(filt5x,EstMethod::moments);
    FWHMx[1] =  fit.get1DEstimates(filt5x,EstMethod::FWHM);
    wx[1]=  fit.compare1DEstimates(mom5x[2],mom5x[3],FWHMx[1][2],FWHMx[1][3]);

    std::vector<double> filt10x = edit.applyFilter(CoIA,10,projAxis::x);
    std::vector<double> mom10x =  fit.get1DEstimates(filt10x,EstMethod::moments);
    FWHMx[2] =  fit.get1DEstimates(filt10x,EstMethod::FWHM);
    wx[2] =  fit.compare1DEstimates(mom10x[2],mom10x[3],FWHMx[2][2],FWHMx[2][3]);

    std::vector<double> filt20x = edit.applyFilter(CoIA,20,projAxis::x);
    std::vector<double> mom20x =  fit.get1DEstimates(filt20x,EstMethod::moments);
    FWHMx[3] =  fit.get1DEstimates(filt20x,EstMethod::FWHM);
    wx[3] =  fit.compare1DEstimates(mom20x[2],mom20x[3],FWHMx[3][2],FWHMx[3][3]);


    //Apply different filters and decid which is best FOR Y
    std::vector<double> noFiltY = edit.applyFilter(CoIA,1,projAxis::y);
    std::vector<double> momy =  fit.get1DEstimates(noFiltY,EstMethod::moments);
    FWHMy[0] =  fit.get1DEstimates(noFiltY,EstMethod::FWHM);
    wy[0] =  fit.compare1DEstimates(momy[2],momy[3],FWHMy[0][2],FWHMy[0][3]);

    std::vector<double> filt5y = edit.applyFilter(CoIA,5,projAxis::y);
    std::vector<double> mom5y =  fit.get1DEstimates(filt5y,EstMethod::moments);
    FWHMy[1] =  fit.get1DEstimates(filt5y,EstMethod::FWHM);
    wy[1] =  fit.compare1DEstimates(mom5y[2],mom5y[3],FWHMy[1][2],FWHMy[1][3]);

    std::vector<double> filt10y = edit.applyFilter(CoIA,10,projAxis::y);
    std::vector<double> mom10y =  fit.get1DEstimates(filt10y,EstMethod::moments);
    FWHMy[2] =  fit.get1DEstimates(filt10y,EstMethod::FWHM);
    wy[2] =  fit.compare1DEstimates(mom10y[2],mom10y[3],FWHMy[2][2],FWHMy[2][3]);

    std::vector<double> filt20y = edit.applyFilter(CoIA,20,projAxis::y);
    std::vector<double> mom20y =  fit.get1DEstimates(filt20y,EstMethod::moments);
    FWHMy[3] =  fit.get1DEstimates(filt20y,EstMethod::FWHM);
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

    std::vector<double> FiltX = edit.applyFilter(CoIA,filter,projAxis::x);

    FWHMx= fit.get1DEstimates(FiltX,EstMethod::FWHM);

    std::vector<double> FiltY = edit.applyFilter(CoIA,filter,projAxis::y);

    FWHMy =  fit.get1DEstimates(FiltY,EstMethod::FWHM);

    //Get the estimates that correspond to those minimum w values and output the corresponding estimates
    std::vector<double> output; //(xA,xB,xMu,xSigma,yA,yB,yMu,ySigma)
    output.insert(output.end(),FWHMx.begin(),FWHMx.end());
    output.insert(output.end(),FWHMy.begin(),FWHMy.end());

    return output;

}

void offlineImageAnalyser::crop(const int& x,const int& y,const int& w,const int& h){
    //CROP IMAGE
    CoIA = edit.crop(CoIA,x,y,w,h);
    std::cout<<"Finished cropping"<<std::endl;

}
//Functions to set ES bools
void offlineImageAnalyser::useBackground(const bool& tf){
   CoIA.useBkgrnd=tf;
}
void offlineImageAnalyser::useManualCrop(const bool& tf){
    CoIA.useManualCrop=tf;
}
void offlineImageAnalyser::useESMask(const bool& tf){
    CoIA.useMaskFromES=tf;
}
void offlineImageAnalyser::useESPixToMm(const bool& tf){
    CoIA.usePixToMmFromES=tf;
}
void offlineImageAnalyser::useESRRThreshold(const bool& tf){
    CoIA.useRRThresholdFromES=tf;
}
void offlineImageAnalyser::useESSigmaCut(const bool& tf){
    CoIA.useSigmaCutFromES=tf;
}
void offlineImageAnalyser::useESFilter(const bool& tf){
    CoIA.useFilterFromES=tf;
}
void offlineImageAnalyser::useESDirectCut(const bool& tf){
    CoIA.useDirectCutLevelFromES=tf;
}
// Functions to set ES data
void offlineImageAnalyser::setManualCrop(const int& x,const int& y,const int& w,const int& h){
    CoIA.manualCropX=x;
    CoIA.manualCropY=y;
    CoIA.manualCropW=w;
    CoIA.manualCropH=h;
}
void offlineImageAnalyser::setESMask(const int& x,const int& y,const int& rx,const int& ry){
    CoIA.maskXES=x;
    CoIA.maskYES=y;
    CoIA.maskRXES=rx;
    CoIA.maskRYES=ry;
}
void offlineImageAnalyser::setESPixToMm(const double& ptm){
    CoIA.pixToMmES=ptm;
}
void offlineImageAnalyser::setESRRThreshold(const double& rrt){
    CoIA.RRThresholdES=rrt;
}
void offlineImageAnalyser::setESSigmaCut(const double& sc){
    CoIA.sigmaCutES=sc;
}
void offlineImageAnalyser::setESFilter(const int& f){
    CoIA.filterES=f;
}
void offlineImageAnalyser::setESDirectCut(const double& dc){
    CoIA.DirectCutLevelES=dc;
}
double offlineImageAnalyser::getPTMRatio(){
    return CoIA.pixToMM;
}
bool offlineImageAnalyser::isAnalysing(){
    return analysing;
}
