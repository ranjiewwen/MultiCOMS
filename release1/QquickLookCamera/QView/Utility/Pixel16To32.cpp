#include "Pixel16To32.h"

static int  convertBit16ToBit8(const unsigned char* pixel, unsigned char begin)
{
    return ((unsigned char)(*(pixel + 1) >> begin) | ((unsigned short)(*(pixel) << (8 - begin)))) & 0xff;
}

static int  convertBit16ToBit16(const unsigned char* pixel, unsigned char /*begin*/)
{
    return (((*pixel << 8) | *(pixel + 1)) & 0xFFFF);
}

static unsigned int byte2Uint(unsigned char v)
{
    return ((unsigned int)(v) | (v << 16) | (v << 8));
}

bool Pixel16To32::convert(unsigned char *dst, const unsigned char *src, int width, int height, unsigned char alpha) const
{
    //parallel processing...
    for (int h = 0; h < height; ++h)
    {
        for (int w = 0; w < width; ++w)
        {
            *(((unsigned int*)dst) + h*width + w) = (byte2Uint(convertBit16ToBit8((src + ((h*width+w)<<1)), m_covOffset)) | (alpha << 24));
        }
    }
    return true;
}

bool Pixel16To32::point(const unsigned char *src, unsigned int &val, bool ret8BitsVal) const {
    if (!src)
        return false;
    if (ret8BitsVal)
    {
        val = convertBit16ToBit8(src, m_covOffset);
    }
    else
    {
        val = convertBit16ToBit16(src, 0);
    }

    return true;
}
