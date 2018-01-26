#ifndef PIXEL16TO32_H
#define PIXEL16TO32_H

#include "PixelConvertor.h"

class Pixel16To32 : public PixelConvertor
{
    unsigned char m_covOffset{ 8 }; //16bit->8bit: select bit offset
public:
    virtual bool convert(unsigned char* dst, const unsigned char* src, int width, int height, unsigned char alpha = 0xff) const override;
    virtual bool point(const unsigned char *src, unsigned int &val, bool ret8BitsVal) const override;

    void setConvertOffset(unsigned char off) { m_covOffset = off; }
    unsigned char convertOffset() const { return m_covOffset; }
};

#endif // PIXEL16TO32_H
