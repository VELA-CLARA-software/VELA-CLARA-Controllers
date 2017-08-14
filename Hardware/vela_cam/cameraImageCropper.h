#ifndef CAMERAIMAGECROPPER_H
#define CAMERAIMAGECROPPER_H
// djs
#include <velaCamStructs.h>
// stl
#include <vector>
#include <string>
//
class cameraImageCropper
{
    public:
        cameraImageCropper( );
        virtual ~cameraImageCropper();

        std::vector< unsigned char > getMask( std::string & screen); /// NB masks assume cropped images!

        velaCamStructs::imgDatC cropImageToScreen(const std::vector< unsigned char > & data, std::string & screen );

        velaCamStructs::imgDatC cropImage(const std::vector< unsigned char > & data, velaCamStructs::imageCutStruct & icsData  );
        velaCamStructs::imgDatD cropImage(const std::vector< double > & data, velaCamStructs::imageCutStruct & icsData  );

        std::vector< double > getXProj( velaCamStructs::imgDatC & idc );
        std::vector< double > getXProj( velaCamStructs::imgDatD & idc );
        std::vector< double > getYProj( velaCamStructs::imgDatC & idc );
        std::vector< double > getYProj( velaCamStructs::imgDatD & idc );


        void applyMask( std::vector< unsigned char > & data, std::string & screen );

    protected:
    private:


        void sanityCheckImageCutStuct( velaCamStructs::imageCutStruct & icsData );





        /// Each screen has a mask

        std::map< std::string, std::vector< unsigned char > > maskData;
        std::map< std::string, velaCamStructs::imgCutStruct  > icsData;

        bool inEllipse( velaCamStructs::imgCutStruct & ics, size_t xCoord, size_t yCoord, double h, double k);
        void calcMask( std::string & screen);

//        void checkICSData( std::string & screen );

};
#endif // CAMERAIMAGECROPPER_H
