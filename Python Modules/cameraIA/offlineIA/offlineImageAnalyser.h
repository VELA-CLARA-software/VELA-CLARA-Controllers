
#ifndef OFFLINE_IMAGE_ANALYSER_H
#define OFFLINE_IMAGE_ANALYSER_H


#include<string>
#include<vector>


#include"cameraStructs.h"
#include"editor.h"
#include"fitter.h"
#include "baseObject.h"

///---------------------------CLASS TO ANALYSE IMAGES------------------------------///
///------------Holds data of image high level imageAnalysis functions-------------///
class offlineImageAnalyser: public baseObject{
    public:
        //Constructor
        offlineImageAnalyser(const bool show_messages = true,
                             const  bool show_debug_messages = true);
        //Destructor
        ~offlineImageAnalyser();

        //get access to lots of functions
        editor edit;
        fitter fit;

        const cameraOfflineIAObject data;

        ///Functions

        void loadImage(const std::vector<double> originalImage);
        void loadBackground(const std::vector<double> originalBkgrndImage);

        void crop(const int& x,const int& y,const int& w,const int& h);

        std::vector<double> analyse();

        std::vector<double> getBest1DParmaetersForXAndY();
        //for use with a specific filter
        std::vector<double> get1DParmaetersForXAndY(const int& filter);

        ///Function to be sent up access data in python

        void useBackground(const bool& tf);
        void useManualCrop(const bool& tf);
        void setManualCrop(const int& x,const int& y,const int& w,const int& h);
        void useESMask(const bool& tf);
        void setESMask(const int& x,const int& y,const int& rx,const int& ry);
        void useESPixToMm(const bool& tf);
        void setESPixToMm(const double& ptm);
        double getPTMRatio();
        void useESRRThreshold(const bool& tf);
        void setESRRThreshold(const double& rrt);
        void useESSigmaCut(const bool& tf);
        void setESSigmaCut(const double& sc);
        void useESFilter(const bool& tf);
        void setESFilter(const int& f);
        void useESDirectCut(const bool& tf);
        void setESDirectCut(const double& dc);


    protected:

    private:
};

#endif
