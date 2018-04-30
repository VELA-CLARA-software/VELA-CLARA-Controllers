
#ifndef OFFLINE_IMAGE_ANALYSER_H
#define OFFLINE_IMAGE_ANALYSER_H


#include<string>
#include<vector>
#include <chrono>

#include"cameraStructs.h"
#include"editor.h"
#include"fitter.h"
#include "baseObject.h"

#include "cameraIAInterface.h"

using namespace cameraStructs;
///---------------------------CLASS TO ANALYSE IMAGES------------------------------///
///------------Holds data of image high level imageAnalysis functions-------------///
class offlineImageAnalyser: public baseObject{
    public:
        //Constructor
        offlineImageAnalyser(bool& show_messages,
                             bool& show_debug_messages,
                             cameraIAInterface* CI = &cameraIAInterface());
        //Destructor
        ~offlineImageAnalyser();

        cameraOfflineIAObject CoIA;

        ///Functions
        void loadImage(const std::vector<double> &originalImage,
                       const std::string &name,
                       int hieght, int width);
#ifdef BUILD_DLL
        void loadImage_Py(const boost::python::list &originalImage,
                       const std::string &name,
                       int hieght, int width);
        void loadBackgroundImage_Py(const boost::python::list &originalBkgrndImage, const std::string &name);
#endif
        void loadBackgroundImage(const std::vector<double> &originalBkgrndImage, const std::string &name);
        void writeData(const std::string &fileName);
        void analyse();
        bool staticAnalyse();
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
        bool isAnalysing();
        bool killLongRunningThread(const double& timeOut);


    protected:

    private:
        cameraIAInterface* cameraInterface;
        std::chrono::steady_clock::time_point startTime,stopTime;
        std::thread *threadedAnalysis;
        bool analysing=false;
        std::vector<double> getBest1DParmaetersForXAndY();
        //for use with a specific filter
        std::vector<double> get1DParmaetersForXAndY(const int& filter);
        //get access to lots of functions
        editor edit;
        fitter fit;
        void crop(const int& x,const int& y,const int& w,const int& h);
};

#endif
