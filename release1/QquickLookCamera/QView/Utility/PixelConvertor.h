#ifndef PIXELCONVERTOR_H
#define PIXELCONVERTOR_H


class PixelConvertor
{
public:
    PixelConvertor();
    virtual ~PixelConvertor();
    virtual bool convert(unsigned char* dst, const unsigned char* src, int width, int height, unsigned char alpha = 0xff) const = 0;
    virtual bool point(const unsigned char* src, unsigned int& val, bool ret8BitsVal = false) const = 0;
};

#endif // PIXELCONVERTOR_H
