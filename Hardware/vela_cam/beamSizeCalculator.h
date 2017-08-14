#ifndef BEAM_SIZE_CALCULATOR_H
#define BEAM_SIZE_CALCULATOR_H
// djs
#include "emitDAQStructs.h"
#include "velaCamStructs.h"

    /// We will assume this class takes raw data in the form of an emitDAQStructs::emitScanSaveDataStruct
    /// We will assume all the data is for the same screen ( ! ) adding flexibility here makes things too complicated
    /// We will be very prescriptive.
    /// beamDat is data with beam
    /// backDat is data with no beam
    /// 1. crop beamDat & backDat to screen, i.e. 2 x mask x and y rad
            /// If no backgrounds, or flags set, calc commonest element for each cropped image
            /// If flags set subtract commonest values from each cropped image
    /// 2. Apply mask, this zeros everything outside of the 'user defined' screen ellipse
    /// 3. If background images:
        /// Calc meanbackground
        /// Subtract meanbackground from each data image, dataMinusBackGround


class cameraImageCropper;
class TH2D;
class  TF2;

class beamSizeCalculator
{
    public:
        beamSizeCalculator();
        ~beamSizeCalculator();


        void setData( emitDAQStructs::emitScanSaveDataStruct dataIN ){ rawData = dataIN; }

        velaCamStructs::projectedMomentsStruct getProjectedMoments(  velaCamStructs::imgDatD );
        velaCamStructs::projectedMomentsStruct getProjectedMoments(  velaCamStructs::imgDatC );

        void cropRawDataToScreen();

        void applyMask();

        void getSaturatedPositions();

        void subtractBackGround();

        void calcMeanBackground();

        void cropTo2FWHM();

        void fitBVNData();

        /// These get passed to the root fitting class, so they have to be static
        /// Or they could go somewhere else...
        /// I'm following the example from here https://root.cern.ch/root/html532/tutorials/fit/fit2.C.html

        static double bvnExpression(double *coords, double *par);

        void plot1DProj( velaCamStructs::imgDatD & id );

    protected:
    private:

        std::vector< velaCamStructs::imgDatC > beamDatCroppedToScreen, backDatCroppedToScreen;

        std::vector< velaCamStructs::imgDatD > dataMinusBackGround, dataToFit;

        std::vector< velaCamStructs::bvnFit  > dataFits;

        velaCamStructs::imgDatD meanBackGround;

        unsigned char satValue;
        std::vector< std::vector< int > > saturatedPositions;
        std::vector< int > getSaturatedPositions( std::vector< unsigned char > & data, unsigned char  value );

        void cropImage( emitDAQStructs::emitScanSaveDataStruct dataStruct );
        void fillBeamSizeDataStruct( emitDAQStructs::emitScanSaveDataStruct & dataStruct );


        emitDAQStructs::emitScanSaveDataStruct rawData;

        emitDAQStructs::beamSizeDataStruct beamSizeData;

        cameraImageCropper * imageCropper;

        void fitTo1DProj( emitDAQStructs::beamSizeDataStruct & bsd );
        velaCamStructs::bvnFit fitToBVN( velaCamStructs::imgDatD & ics, std::vector< int > & satPos  );


        TH2D * fitHist;
        TF2  * bvnFunction;

        //void setColorScheme();

        void cut2FWHM( std::vector< double > & vec, int & cen, int & rad );


        bool shouldGetCommonestFLAG, shouldSubtractCommonestFLAG, haveDataToFit;

        bool shouldSubtractCommonest(){ return shouldSubtractCommonestFLAG; }
        bool shouldGetCommonest(){ return shouldGetCommonestFLAG; }

        void setShouldGetCommonest( bool val){ shouldGetCommonestFLAG = val; }
        void setShouldSubtractCommonest( bool val){ shouldSubtractCommonestFLAG = val; }


        struct by_second
        {
            template <typename Pair>
            bool operator()(const Pair& a, const Pair& b)
            {
                return a.second < b.second;
            }
        };

        std::vector< int > croppedCommenstValues;

        unsigned char getMostCommonElement(std::vector< unsigned char >::iterator begin, std::vector< unsigned char >::iterator end);


       // emitDAQStructs::emitScanSaveDataStruct dataStruct;

//        ClassDef(emitDAQController, 0 ); /// root
};

#endif // EMITDAQCONTROLLER_H
