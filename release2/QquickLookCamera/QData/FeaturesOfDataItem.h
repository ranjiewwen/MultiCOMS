#ifndef FEATURESOFDATAITEM_H
#define FEATURESOFDATAITEM_H

//point size , bytes per pixel
//enum class POINT_SIZE
//{
//    SIZE_1 = 1,
//    SIZE_2 = 2,
//    SIZE_4 = 4
//};

class FeaturesOfDataItem
{
public:
    FeaturesOfDataItem();

    //basic information
    int linesPerFrame{ 1024 }; //height
    int attachedDataWidth {0}; //addtional data width
    int payloadDataWidth{0}; //entire image or valid data width
    //int validPayloadWidth{0}; //current loaded image or valid data width
    //int validPayloadOffset{0}; //x offset
    //POINT_SIZE attachedDataSize{POINT_SIZE::SIZE_1}; //unit
    //POINT_SIZE payloadDataSize{POINT_SIZE::SIZE_1}; //unit

    //additional information
    //int colSampleLevel{1}; //image or valid data : column sample level
    //int rowSampleLevel{1}; //image or valid data : row sample level

    int dataChannel{-1}; //channel
};

#endif // FEATURESOFDATAITEM_H
