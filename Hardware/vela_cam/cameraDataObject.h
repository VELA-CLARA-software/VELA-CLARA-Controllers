#ifndef CAMERADATAOBJECT_H
#define CAMERADATAOBJECT_H
// djs
#include <velaCamStructs.h>
// stl
#include <vector>
#include <string>
//
class cameraDataObject
{
    public:
        cameraDataObject( std::string & name );
        virtual ~cameraDataObject();

    /// Loads dummy data for offline testing

        bool setDataToDebug();

    /// Get raw data reference or pointer to part of raw data

        std::vector< unsigned char > & getRawRef ( velaCamStructs::DATA_TYPE type );
        std::vector< double >        & getProcRef( velaCamStructs::DATA_TYPE type );

        unsigned char * getRawRef ( velaCamStructs::DATA_TYPE type, size_t part );
        double        * getProcRef( velaCamStructs::DATA_TYPE type, size_t part );

        void resetRaw( velaCamStructs::DATA_TYPE type,  size_t numShots );

        void setDataAsBack();

        bool process( velaCamStructs::DATA_TYPE type );

        size_t getNumPix();

        velaCamStructs::imgCutStruct  getIMGCutParam();
        void setIMGCutParam( velaCamStructs::imgCutStruct & p);

        void setDataIsSet( bool v );
        void setBackIsSet( bool v );
        void setDataIsCut( bool v );

        bool DataIsSet();
        bool DataIsCut();
        bool BackIsSet();
        bool BackIsCut();

        bool testForSaturation();

    protected:
    private:

        bool dataIsSet, backIsSet, dataIsCut, backIsCut, maskSet, cutParamSet, dataIsSaturated;

        velaCamStructs::camDataStruct dataStruct;

        std::vector< unsigned char > data, back;
        std::vector< double >        procData, procBack, mask;

        size_t dataPos, backPos, numIts; /// used for adding data to existing vetors, rather than resizing and adding from start

        bool cut( velaCamStructs::DATA_TYPE type );
        bool positiveCutValues();

        bool applyMask( velaCamStructs::DATA_TYPE type );
        bool getMask();
        bool inEllipse( size_t xCoord, size_t yCoord, double h, double k);

        bool DataIsNotSet();
        bool DataIsNotCut();
        bool BackIsNotSet();
        bool BackIsNotCut();
        bool MaskSet();
        bool MaskNotSet();


};
#endif // CAMERADATAOBJECT_H
