#include <fstream>
#include <iterator>
#include <vector>
#include <stdint.h>
#include<iostream>
#include<sstream>
//#include <algorithm>
#include <cmath>

#include"TF1.h"
#include"TF2.h"
#include"TH2F.h"
#include"TGraph.h"
#include"TMath.h"
#include"TFormula.h"
#include <TMath.h>
#include <TGraph2D.h>

//#include <TCanvas.h>
#include"TFitResultPtr.h"
#include"TFitResult.h"
#include"TDirectory.h"
#include"TVirtualFitter.h"
#include"TFitter.h"
#include"editor.h"
#include"fitter.h"


///--------------------------------FIT FUNCTIONS------------------------------------///
//CONSTRUCTOR
fitter::fitter( bool* show_messages_ptr, bool * show_debug_messages_ptr )
: baseObject( show_messages_ptr, show_debug_messages_ptr ),
editFunctions( show_messages_ptr, show_debug_messages_ptr )
{}
//DESTRUCTOR
fitter::~fitter(){}

//1D NORMAL EST(returns A,B MU, AND SIGAMA) CHOOSE PROJECTION AND WHETHER TO USE MOMENTS OR FWHM
std::vector<double> fitter::get1DEstimates(const std::vector<double>& projection,
                                           const EstMethod& method){
    std::vector<double> parameters(4,0);
    std::vector<double> temp = projection;
    int projectionSize = projection.size();
    std::vector<double>::iterator minValue = std::min_element(std::begin(temp),std::end(temp));

    //get A
    std::vector<double>::iterator maxVlaue = std::max_element(std::begin(temp),std::end(temp));
    parameters[0]=*maxVlaue;

    //get B (average over point ehat areof less that a third the max value)
    double counter =0,sum=0;
    for(auto i=0;i<projectionSize;++i){
        if(temp[i]<*maxVlaue/3){
            sum+=temp[i];
            counter++;}
    }
    parameters[1]=sum/counter;

    //parameters[1]=average(temp);

    switch(method)
    {
        case EstMethod::FWHM:
        {
             double halfMax = *maxVlaue/2;
             double counter=0;
            //get mu
            parameters[2] = std::distance(std::begin(temp), maxVlaue);
            //get sigma
            for(auto i=0;i<projectionSize;++i){
                if(temp[i]>halfMax){counter++;}
            }
            parameters[3] = counter/2;
            break;
        }

        case EstMethod::moments:
        {
            //shift proctection up so no negatives
            if(*minValue<0){
                for(auto i=0; i<projectionSize;++i){
                    temp[i]+=abs(*minValue);
                }
            }

           double sumTemp=0;
           for (auto i : temp){
                sumTemp += i;
           }

            //get mu
            double expectance=0;
            for(auto i=0;i<projectionSize;++i){
                expectance+=(temp[i]/sumTemp)*i;
            }
            parameters[2] = expectance;
           //get Variance
            double variance=0;
            for(auto i=0;i<projectionSize;++i){
                variance+=pow(i-expectance,2)*(temp[i]/sumTemp);
            }
            //get sigma
            parameters[3] = sqrt(variance);
            break;
        }
    }
    return parameters;
}
// COMPARE 1D MU AND SIGMA VALUES OF TWO ESTIMATES
double fitter::compare1DEstimates(const double & mu1,const double & sigma1,
                                  const double & mu2,const double & sigma2){
    return (abs(mu1-mu2)/(mu1+mu2))+(abs(sigma1-sigma2)/(sigma1+sigma2));
}
//1D GAUSSIAN FIT To PROJECTION
std::vector<double> fitter::fit1DGaussianToProjection(const cameraOfflineIAObject& ID,
                                                      const projAxis& axis,
                                                      const std::vector<double>& estimates){
    std::vector<double> out;
    switch(axis){
        case projAxis::x:{
            out=fitGuassian(ID.xProjection,estimates);
            break;
        }
        case projAxis::y:{
            out=fitGuassian(ID.yProjection,estimates);
            break;
        }
    }
    return out;
}

//GENERAL 1D GUASSAIN FIT
std::vector<double> fitter::fitGuassian(const std::vector<double> data,
                                        const std::vector<double> estimates){
            int n = data.size();
            TGraph gr = TGraph();
            for(auto i=0;i<n;++i){
                    gr.SetPoint(i,i,data[i]);
            }
            TF1 func = TF1("func","[0]*exp(-0.5*((x-[2])/[3])**2)+[1]",0,n);
            func.SetParameters(estimates[0],estimates[1],estimates[2],estimates[3]);
            gr.Fit("func","Q");
            std::vector<double>out={func.GetParameter(0),func.GetParameter(1),func.GetParameter(2),func.GetParameter(3)};
            return out;
}
//USE 1D FITS TO DO A BVN FIT
std::vector<double> fitter::fitBVN(const cameraOfflineIAObject& ID,
                                   const std::vector<double>& xEstimates,
                                   const std::vector<double>& yEstimates){
    std::vector<double> dummyIntensity = ID.rawData;
    int n=ID.rawData.size();
    int w=ID.xProjection.size();
    int h=ID.yProjection.size();

    ///https://root.cern.ch/download/doc/primer/ROOTPrimer.html
    TFitter::SetDefaultFitter("Minuit");
    TFitter::SetPrecision(0.00000001);
    //TFitter::SetMaxIterations(50000000);
    TGraph2D gr =  TGraph2D();


    for(auto i=0;i<n;++i){
         gr.SetPoint(i,ID.xPos[i],ID.yPos[i],ID.rawData[i]);
    }
//    TH2F gr =  TH2F();
//
//    for(auto i=0;i<n;++i){
//        gr.Fill(ID.X[i],ID.Y[i],ID.rawData[i]);
//    }
    std::vector<double>::iterator MaxValue = std::max_element(std::begin(dummyIntensity),std::end(dummyIntensity));
    //"[1] + [0]*exp(-0.5*([5]*(x-[2])**2-2*[6]*(x-[2])*(y-[3])+[4]*(y-[3])**2)/([4]*[5]-[6]**2))"
    TF2 bvn = TF2("bvn","[1] + [0]*exp(-0.5*([5]*(x-[2])**2-2*[6]*(x-[2])*(y-[3])+[4]*(y-[3])**2)/([4]*[5]-[6]**2))",0,w,0,h);
    bvn.SetParName(0, "Amp");
    //bvn.SetParameter(0,*MaxValue); //cant use for histograms
    bvn.SetParName(1, "Bkgrnd");
    bvn.SetParameter(1,0);// xEstimates[1]);
    bvn.SetParName(2, "muX");
    bvn.SetParameter(2, xEstimates[2]);
    bvn.SetParName(3, "muY");
    bvn.SetParameter(3, yEstimates[2]);
    bvn.SetParName(4, "sigma11");
    bvn.SetParameter(4, xEstimates[3]*xEstimates[3]);

    bvn.SetParName(5, "sigma22");
    bvn.SetParameter(5, yEstimates[3]*yEstimates[3]);
    bvn.SetParName(6, "sigma12");
    bvn.SetParameter(6, 0);

    gr.Fit("bvn","F");
    std::vector<double> bvnParameters={(double)bvn.GetParameter(0),
                                       (double)bvn.GetParameter(1),
                                       (double)bvn.GetParameter(2),
                                       (double)bvn.GetParameter(3),
                                       (double)bvn.GetParameter(4),
                                       (double)bvn.GetParameter(5),
                                       (double)bvn.GetParameter(6),
                                       (double)bvn.GetParError(0),
                                       (double)bvn.GetParError(1),
                                       (double)bvn.GetParError(2),
                                       (double)bvn.GetParError(3),
                                       (double)bvn.GetParError(4),
                                       (double)bvn.GetParError(5),
                                       (double)bvn.GetParError(6)};

    return bvnParameters;

}
//DIRECTLY CALCULATE THE MOMENT OF THE IMAGE DISTRIBUTION
std::vector<double> fitter::covarianceValues(const cameraOfflineIAObject& ID,
                                             const double & cut,
                                             const double & pixelValueError){
    int ylen = ID.yProjection.size();
    int xlen = ID.xProjection.size();
    int dataLength = ID.rawData.size();
    double pixelError = pixelValueError;
    //Trial using Chris T method
    double sumPixIntensity=0;
    std::vector<double> dummyIntensity = ID.rawData;
    std::vector<double>::const_iterator M = std::max_element(dummyIntensity.begin(), dummyIntensity.end());
    for(auto i=0;i<dataLength;++i){
        if(dummyIntensity[i]<(cut/100)*(*M)){dummyIntensity[i]=0;}
        sumPixIntensity+=dummyIntensity[i];
    }
    double sumPixIntensityError = sqrt(dataLength*pixelError*pixelError);

    std::vector<double> dataV;
    for(auto i=0; i<dataLength;++i){
        dataV.push_back(dummyIntensity[i]/sumPixIntensity);
    }
    //std::vector<double> xD = ID.X;
   // std::vector<double> yD = ID.Y;
/// For error propaation see:
    //GET muX
    double muX=calculateMu(dataV,ID.xPos);
    //GET muX error
    double xPosSquared=0;
    for(auto i=0; i<dataLength;++i){
        xPosSquared+=pow(ID.xPos[i],2);
    }
    double muXError = sqrt(pow(muX/sumPixIntensity,2)*pow(sumPixIntensityError,2)+
                            pow(pixelError/sumPixIntensity,2)*xPosSquared);
    //GET muY
    double muY=calculateMu(dataV,ID.yPos);
    //GET muY error
    double yPosSquared=0;
    for(auto i=0; i<dataLength;++i){
        xPosSquared+=pow(ID.yPos[i],2);
    }
    double muYError = sqrt(pow(muY/sumPixIntensity,2)*pow(sumPixIntensityError,2)+
                            pow(pixelError/sumPixIntensity,2)*yPosSquared);
    //GET sigmaXSquared
    double s11=calculateSigmaSquared(dataV,ID.xPos,muX);
    //GET sigmaXSquared error
    double sum1=0;
    double sum2=0;
    for(auto i=0; i<dataLength;++i){
        sum1+=dummyIntensity[i]*(ID.xPos[i]-muX);
        sum2+=pow((ID.xPos[i]-muX),2);
    }
    double s11Error = sqrt(pow(s11/sumPixIntensity,2)*pow(sumPixIntensityError,2)+
                            pow((2*sum1)/sumPixIntensity,2)*pow(muXError,2)+
                            pow(sum2/sumPixIntensity,2)*pow(pixelError,2));
    //GET sigmaYSquared
    double s22=calculateSigmaSquared(dataV,ID.yPos,muY);
    //GET sigmaYSquared error
    double sum3=0;
    double sum4=0;
    for(auto i=0; i<dataLength;++i){
        sum3+=dummyIntensity[i]*(ID.yPos[i]-muY);
        sum4+=pow((ID.yPos[i]-muY),2);
    }
    double s22Error = sqrt(pow(s22/sumPixIntensity,2)*pow(sumPixIntensityError,2)+
                            pow((2*sum3)/sumPixIntensity,2)*pow(muYError,2)+
                            pow(sum4/sumPixIntensity,2)*pow(pixelError,2));
    //GET CovXY
    double s12=calculateSigmaSquared(dataV,ID.xPos,ID.yPos,muX,muY);
    //GET CovXY error
    double sum5=0;
    for(auto i=0; i<dataLength;++i){
        sum5+=(ID.yPos[i]-muY)*(ID.xPos[i]-muX);
    }
    double s12Error = sqrt(pow(s12/sumPixIntensity,2)*pow(sumPixIntensityError,2)+
                           pow(sum3/sumPixIntensity,2)*pow(muXError,2)+
                           pow(sum1/sumPixIntensity,2)*pow(muYError,2)+
                           pow(sum5/sumPixIntensity,2)*pow(pixelError,2));
    std::vector<double> out = {muX,muY,s11,s22,s12,
                               muXError,muYError,s11Error,s22Error,s12Error};
    return out;
}
//CALC MU
double fitter::calculateMu(const std::vector<double> &data,
                           const std::vector<double> &position){
    return editFunctions.dotProduct(data, position);
}

//CALC SIGMA SQUARED
double fitter::calculateSigmaSquared(const std::vector<double> &data,
                                     const std::vector<double> &position,
                                     const double &mu){
    double out=0;
    for(auto i=0; i!=data.size();++i){
        out+=data[i]*pow(position[i]-mu,2);
    }
    return out;
}
//CALC SIGMA SQUARED overidden
double fitter::calculateSigmaSquared(const std::vector<double> &data,
                                     const std::vector<double> &position1,
                                     const std::vector<double> &position2,
                                     const double &mu1, const double &mu2){
    double out=0;
    for(auto i=0; i!=data.size();++i){
        out+=data[i]*(position1[i]-mu1)*(position2[i]-mu2);
    }
    return out;
}
//GET R SQUARED OF 1d PROJECTIONS
double fitter::rSquaredOf1DProjection(const cameraOfflineIAObject& ID,
                                      const std::vector<double>& parameters,
                                      const projAxis& axis){
    std::vector<double> data;
    switch(axis){
        case projAxis::x:{
            data=ID.xProjection;
            break;
        }
        case projAxis::y:{
            data=ID.yProjection;
            break;
        }

    }

    std::vector<double> fitLine;
    int dataLength=data.size();
    for(auto i=0;i<dataLength;++i){
        fitLine.push_back(parameters[0]*exp(-0.5*pow(((i-parameters[2])/parameters[3]),2))+parameters[1]);
    }
///USE AVERAGE FROM BASE CLASS
    double dataAverage = editFunctions.average(data);

    double SStot=0,SSres=0;

     for(auto i=0;i<dataLength;++i){
        SStot+=pow((data[i]-dataAverage),2);
        SSres+=pow((data[i]-fitLine[i]),2);
     }

     return 1-(SSres/SStot);

}




