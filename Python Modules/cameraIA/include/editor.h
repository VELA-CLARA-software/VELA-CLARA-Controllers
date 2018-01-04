#ifndef EDIT_H
#define EDIT_H

#include"cameraStructs.h"
#include "baseObject.h"

///------------------CLASS TO PUT EDITTING FUNCTIONS----------------------///
///------------Functions that will read in image data from----------------///
///------------image Analyser and output an editted version---------------///
using namespace cameraStructs;

class editor: public baseObject{
    public:
        //Constructor
        editor( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr );
        //Destructor
        ~editor();

        ///Functions
        //APPLY MASK
        std::vector<double> applyMask(const cameraOfflineIAObject& ID);
        //CROP IMAGE
        cameraOfflineIAObject crop(const cameraOfflineIAObject& ID,
                                   const int& x,const int& y,const int& w,const int& h);
        //SUBTRACT BACKGROUND IMAGE
        std::vector<double> subtractBackground(const cameraOfflineIAObject& ID);
        //SUBTRACT IMAGES
        std::vector<double> subtractImages(const std::vector<double>& v1,
                                           const std::vector<double>& v2);
        //CHANGE INTENSITY OF IMAGE BY A FACTOR
        std::vector<double> scaleImage(const double& A,
                                       const std::vector<double>& v1);
        //SCALED MASK SUBTRACTION
        std::vector<double> scaledMaskSubtraction(const cameraOfflineIAObject& ID);
        //N POINT SCALING
        std::vector<double> nPointScaling(const cameraOfflineIAObject& ID);
        // APPLY FILTER MOVING AVERAGE (FOR PROJECTIONS) 5,10 or 20
        std::vector<double> applyFilter(const cameraOfflineIAObject& ID,
                                        const int& A, const  projAxis& axis);
        //FILTER METHOD
        std::vector<double> filterMethod(const std::vector<double> &v,
                                         const int& A);
        //GET CORRECTIONS TO BEAM CENTROID
        std::vector<double> correctBeamPosition(const double& muX,
                                                const double& muY);
        //PIXTOMM
        double ptm(const double & ptmRatio, const double& valInPixels);
        //these keep track of how much the image has been cropped and used to rescale the centroid position later
        int croppedX=0,croppedY=0;
        //MAKE MASK
        std::vector<double> makeMask(const cameraOfflineIAObject& ID,
                                     const int& x0, const int& y0,
                                     const int& rX, const int& rY);
        //GET PROJECTION
        std::vector<double> getProjection(const cameraOfflineIAObject& ID,
                                          const  projAxis &axis);

        //MAKE POJECTION
        std::vector<double> makeProjection(const std::vector<double> &v,
                                           const int &lengthOfFirstSum,
                                           const int &lengthOfSecondSum,
                                           const int &width, const bool &revIndices);
        std::vector<double> subtractVectors(const std::vector<double>& v1,const std::vector<double>& v2);
        double dotProduct(const std::vector<double>& v1,const std::vector<double>& v2);
        double average(const std::vector<double>& v1);
    protected:

    private:
};



#endif

