#ifndef IDATAITEM_H
#define IDATAITEM_H

#include "FeaturesOfDataItem.h"
#include <memory>

class IDataItem
{
protected:
    std::shared_ptr<FeaturesOfDataItem> m_features{nullptr};  //features of data
public:
    IDataItem();
    virtual ~IDataItem() = 0;

    //set entire image width, assist data width and image&assist height(display)
    virtual bool setup(int assWidth, int imgWidth, int height)
    {
        m_features->attachedDataWidth = assWidth;
        m_features->payloadDataWidth = imgWidth;
        m_features->linesPerFrame = height;
        return true;
    }
    ////set column sample level
    //virtual bool setColSampleLevel(int colSampleLevel)
    //{
    //    m_features->colSampleLevel = colSampleLevel;
    //    return true;
    //}
    ////set row sample level
    //virtual bool setRowSampleLevel(int rowSampleLevel)
    //{
    //    m_features->rowSampleLevel = rowSampleLevel;
    //    return true;
    //}
    ////set current offset&width of image output
    //virtual bool setValidImageOffset(int offset, int validWidth)
    //{
    //    if (!m_features)
    //        return false;
    //    if (offset < 0 || validWidth<0 || validWidth>m_features->payloadDataWidth)
    //        return false;
    //    if (offset + validWidth > m_features->payloadDataWidth)
    //        return false;
    //    m_features->validPayloadOffset = offset;
    //    m_features->validPayloadWidth = validWidth;
    //    return true;
    //}


    //features of data include assist(if exists)&image data
    virtual const FeaturesOfDataItem* constDataFeatures() const {return m_features.get();}
    //copy area of image data
    virtual bool copyArea(int x, int y, int w, int h, unsigned char* extPtr) const = 0;
    ////get entire image data ptr
    //virtual bool internalImageDataPtr(const unsigned char*& ptr) const = 0;
    ////get entire assist data ptr
    //virtual bool internalAssistDataPtr(const unsigned char*& ptr) const = 0;
};

#endif // IDATAITEM_H
