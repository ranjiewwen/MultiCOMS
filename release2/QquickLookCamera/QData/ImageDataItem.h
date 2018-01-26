#pragma once

#include "IDataProcessUnit.h"
#include "IDataItem.h"

#define FRM_SYNWORD 0x499602D2

class ImageDataItem : public IDataItem, public IDataProcessUnit
{
protected:
    std::shared_ptr<unsigned char> m_dualImageBuffer[2]; //entire image data buffer, dual
    int m_curLineCnt; //channel 4 line counter
    long long m_newFrame{-1}; //frame counter
	long long m_curFrameCnt{ -1 }; //
protected:
    virtual void process() override; //inherit from IDataProcessUnit

protected:
    void storePayloadData(const unsigned char* buf);
	void resetCounter()
	{
		m_newFrame = m_curLineCnt = -1;
	}
public:
    ImageDataItem();
    ~ImageDataItem();

    //inherit from IDataItem
    virtual bool setup(int assWidth, int imgWidth, int height) override;
    //copy area of image data
    virtual bool copyArea(int x, int y, int w, int h, unsigned char* extPtr) const override;

public:
    unsigned int currentFrameCount() const {return m_curFrameCnt;}
};

#pragma pack (push)
#pragma pack (1)
struct CMOS_FRAME_HEAD
{
	//20 Bytes
	unsigned int synWord{ 0x499602D2 };//帧同步字
	unsigned int dgramCount;//包计数，htonl
	unsigned int channelID;//镜头标识
	unsigned int frmCount;//帧计数
	unsigned int lineCount;//行标识
};
#pragma pack (pop)
