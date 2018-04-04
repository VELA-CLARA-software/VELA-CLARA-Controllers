#include <fstream>
#include <iterator>
#include <vector>
#include <stdint.h>
#include<iostream>
#include<sstream>
#include <algorithm>
#include <cmath>

#include"editor.h"

///--------------------------------EDIT FUNCTIONS------------------------------------///
//CONSTRUCTOR
editor::editor( bool* show_messages_ptr, bool* show_debug_messages_ptr ): baseObject( show_messages_ptr, show_debug_messages_ptr )
{}
//DESTRUCTOR
editor::~editor(){}

//APPLY MASK
std::vector<double> editor::applyMask(const cameraOfflineIAObject& ID){
    std::vector<double> out(ID.dataSize);
    for(auto i=0; i<ID.dataSize;++i){
        out[i]=ID.rawData[i]*ID.mask[i];
    }
    return out;
}
//CROP IMAGE
cameraOfflineIAObject editor::crop(const cameraOfflineIAObject& ID,
                                            const int& x,const int& y,
                                            const int& w,const int& h){
    cameraOfflineIAObject dummyData;
    dummyData=ID;
    //if statement ignore the cut if it is too big

    if(dummyData.imageHeight>h && dummyData.imageWidth>w){
        std::vector<double>::const_iterator M = std::max_element(ID.rawData.begin(), ID.rawData.end());
        double MM = 2*(*M);
        double WW = 2*ID.imageWidth;
        double HH = 2*ID.imageHeight;
        //highlight the data to remove in crop
        //for( auto && it : dummyData.rawData ){
        int counter =0;
       for(auto i=0;i<ID.dataSize;++i){

            if(ID.xPos[i]<x||ID.xPos[i]>w||ID.yPos[i]<y||ID.yPos[i]>h){

                dummyData.rawData[i] = MM;

                dummyData.mask[i] = MM;

                dummyData.xPos[i] = WW;

                dummyData.yPos[i] = HH;

            }
        }
        //remove highlighted data
        dummyData.rawData.erase(std::remove(begin(dummyData.rawData),end(dummyData.rawData),MM),end(dummyData.rawData));
        dummyData.mask.erase(std::remove(begin(dummyData.mask),end(dummyData.mask),MM),end(dummyData.mask));
        dummyData.xPos.erase(std::remove(begin(dummyData.xPos),end(dummyData.xPos),WW),end(dummyData.xPos));
        dummyData.yPos.erase(std::remove(begin(dummyData.yPos),end(dummyData.yPos),HH),end(dummyData.yPos));
        //Adjust size variable accordingly
        dummyData.imageWidth=w-x+1;
        dummyData.imageHeight=h-y+1;
        dummyData.dataSize=dummyData.rawData.size();

        //Reset xy coordinates () so they start at 00
        for(auto j=0;j<dummyData.dataSize;j++){
            dummyData.xPos[j]-=x;
            dummyData.yPos[j]-=y;
        }
        //take note of cutting
        croppedX+=x;
        croppedY+=y;

    }
    return dummyData;


}
// SUBTRACT BACKGROUN IMAGE
std::vector<double> editor::subtractBackground(const cameraOfflineIAObject& ID){
    std::vector<double> out(ID.dataSize);
    out = subtractImages(ID.rawData,ID.rawBackgroundData);
    return out;
}
//SUBTRACT TWO IMAGES(v1-v2)
std::vector<double> editor::subtractImages(const std::vector<double>& v1,const std::vector<double>& v2){
    return subtractVectors(v1,v2);
}
//CHANGE INTENSITY OF IMAGE BY A FACTOR
std::vector<double> editor::scaleImage(const double& A, const std::vector<double>& v1){
    std::vector<double> v2;
    for(auto i=0;i!=v1.size();++i){
        v2.push_back(A*v1[i]);
    }
    return v2;
}
//SCALED MASK SUBTRACTION
std::vector<double> editor::scaledMaskSubtraction(const cameraOfflineIAObject& ID){
    std::vector<double> out(ID.dataSize);
    //find scaling factor where average intensity is 0
    double scalingFactor[]={-100,100};
    double scaledMeanIntensity[]={0,0};
    double originalAverage = average(ID.rawData);


    scaledMeanIntensity[0]=average(subtractImages(ID.rawData,scaleImage(scalingFactor[0],ID.mask)));
    scaledMeanIntensity[1]=average(subtractImages(ID.rawData,scaleImage(scalingFactor[1],ID.mask)));
    //gradient of line
    double m = (scaledMeanIntensity[1]-scaledMeanIntensity[0])/(scalingFactor[1]-scalingFactor[0]);
    //L0: intersect where average intensity is zero
    double Lambda0 = -originalAverage/m;
    //Apply scaled mask subtraction
    out=subtractImages(ID.rawData,scaleImage(Lambda0,ID.mask));
    return out;
}
//N POINT SCALING (DISCUSTING BUT IT WORKS)
std::vector<double> editor::nPointScaling(const cameraOfflineIAObject& ID){

    std::vector<double> out(ID.dataSize);

    // make vector conataining the first and last 10 point of x and y projections
    std::vector<double> xTopTail(ID.xProjection.begin(),ID.xProjection.begin()+10);
    double averageStartX = average(xTopTail);
    std::vector<double> yTopTail(ID.yProjection.begin(),ID.yProjection.begin()+10);
    double averageStartY = average(yTopTail);
    std::vector<double> endX(ID.xProjection.end()-10,ID.xProjection.end());
    double averageEndX = average(endX);
    std::vector<double> endY(ID.yProjection.end()-10,ID.yProjection.end());
    double averageEndY = average(endY);

    //add tail to topTail vectors
    xTopTail.insert(xTopTail.end(),endX.begin(),endX.end());
    yTopTail.insert(yTopTail.end(),endY.begin(),endY.end());
    //deteremine which lambda to use (x projection or y projection)
    //if top and tail of x proj. have sore similar mean intesities

    std::vector<double> topTailMask;
    std::vector<double> topTail;
    double Average;
    if(abs(averageStartX-averageEndX)<abs(averageStartY-averageEndY)){
        topTail=xTopTail;
        Average = average(xTopTail);
        topTailMask.insert(topTailMask.end(),ID.maskXProjection.begin(),ID.maskXProjection.begin()+10);
        topTailMask.insert(topTailMask.end(),ID.maskXProjection.end()-10,ID.maskXProjection.end());
    }
    else{
        topTail=yTopTail;
        Average = average(yTopTail);
        topTailMask.insert(topTailMask.end(),ID.maskYProjection.begin(),ID.maskYProjection.begin()+10);
        topTailMask.insert(topTailMask.end(),ID.maskYProjection.end()-10,ID.maskYProjection.end());
    }

    double scalingFactor[]={-100,100};
    double scaledMeanIntensity[]={0,0};

    //find Lambda in x axis where average intesity is zero
    scaledMeanIntensity[0]=average(subtractImages(topTail,scaleImage(scalingFactor[0],topTailMask)));
    scaledMeanIntensity[1]=average(subtractImages(topTail,scaleImage(scalingFactor[1],topTailMask)));
    //gradient of line
    double m = (scaledMeanIntensity[1]-scaledMeanIntensity[0])/(scalingFactor[1]-scalingFactor[0]);
    //L0: intersect where average intensity is zero
    double lambda = -Average/m;

    out=subtractImages(ID.rawData,scaleImage(lambda,ID.mask));
    return out;
}
//FIlTER
std::vector<double> editor::applyFilter(const cameraOfflineIAObject& ID, const int& A, const  projAxis& axis){
    switch(axis)
    {
        case projAxis::x:
            {
            return filterMethod(ID.xProjection, A);
            break;
            }
        case projAxis::y:
            {
            return filterMethod(ID.yProjection, A);
            break;
            }
    }
}
//FILTER METHOD
std::vector<double> editor::filterMethod(const std::vector<double> &v, const int& A){
    std::vector<double> out;
    int vSize = v.size();
    for(auto i=0; i<vSize-A+1;++i){
        double dummy=0;
        for(auto j=0;j<A;++j){
           dummy+=v[j+i];
        }
        out.push_back(dummy/A);
    }
    return out;
}
//GET CORRECTIONS TO BEAM CENTROID
std::vector<double> editor::correctBeamPosition(const double& muX, const double& muY){
    std::vector<double> out;
    out.push_back(muX+croppedX);
    out.push_back(muY+croppedY);

    return out;
}
//PIXTOMM (dont really use this as it will be converted on surface level i.e in python GUI)
double editor::ptm(const double & ptmRatio, const double& valInPixels){
    return valInPixels/ptmRatio;
}
//MAKE MASK
std::vector<double> editor::makeMask(const cameraOfflineIAObject& ID,
                                     const int& x0, const int& y0,
                                     const int& rX, const int& rY){
    std::vector<double> out(ID.dataSize);
    for(auto i=0;i!=ID.dataSize;i++){
        //distance of pixel from centre of oval mask
        double x = abs(ID.xPos[i]-x0);
        double y = abs(ID.yPos[i]-y0);
        double r = pow(x/(rX),2)+pow(y/(rY),2);
        if (r>1){out[i]=0.;}
        else{out[i]=1.;}
    }
    return out;
}
//GET PROJECTIONS
std::vector<double> editor::getProjection(const cameraOfflineIAObject& ID, const  projAxis &axis){
    ///This function is used to get projections and update them as analysis is ongoing
    std::vector<double> dummyProjection;
    switch(axis)
    {
        case projAxis::x:
            {
                dummyProjection = makeProjection(ID.rawData,ID.imageWidth,ID.imageHeight,ID.imageWidth,false);
                break;
            }
        case projAxis::y:
            {
                dummyProjection = makeProjection(ID.rawData,ID.imageHeight,ID.imageWidth,ID.imageWidth,true);
                //is read in reverese so have to change that
                std::reverse(dummyProjection.begin(),dummyProjection.end());
                break;
            }
        case projAxis::maskX:
            {
                dummyProjection = makeProjection(ID.mask,ID.imageWidth,ID.imageHeight,ID.imageWidth,false);
                break;
            }
        case projAxis::maskY:
            {
                dummyProjection = makeProjection(ID.mask,ID.imageHeight,ID.imageWidth,ID.imageWidth,true);
                //is read in reverese so have to change that
                std::reverse(dummyProjection.begin(),dummyProjection.end());
                break;
            }
    }
     return dummyProjection;
}
//MAKE PROJECTION
std::vector<double> editor::makeProjection(const std::vector<double> &v,
                                           const int &lengthOfFirstSum,
                                           const int &lengthOfSecondSum,
                                           const int &width, const bool &revIndices){
    std::vector<double> out;
    for(auto i=0; i<lengthOfFirstSum;i++){
        double dummy=0;
        for(auto j=0; j<lengthOfSecondSum;j++){
            if (!revIndices){ dummy+=v[i+(j*width)];}
            else {dummy+=v[j+(i*width)];}
        }
        out.push_back(dummy);
    }
    return out;
}
std::vector<double> editor::subtractVectors(const std::vector<double>& v1,
                                            const std::vector<double>& v2){
    std::vector<double> vOut;
    for(auto i=0;i!=v1.size();++i){
        vOut.push_back(v1[i]-v2[i]);
    }
    return vOut;
}
double editor::dotProduct(const std::vector<double>& v1,
                          const std::vector<double>& v2){
    double sum(0);
    for(auto i=0;i!=v1.size();++i){
        sum += v1[i]*v2[i];
    }
    return sum;
}
double editor::average(const std::vector<double>& v1){
    double sum(0);
    for(auto i=0;i!=v1.size();++i){
        sum += v1[i];
    }
    return sum/double(v1.size());
}






