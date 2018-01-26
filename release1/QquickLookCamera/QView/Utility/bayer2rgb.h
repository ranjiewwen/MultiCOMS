#ifndef BAYER2RGB_H
#define BAYER2RGB_H

#include"PixelConvertor.h"

#define B(x,y) dst[0 + 3 * ((x) + width * (y))]
#define G(x,y) dst[1 + 3 * ((x) + width * (y))]
#define R(x,y) dst[2 + 3 * ((x) + width * (y))]
#define Bay(x,y) src[(x) + width * (y)]

#define u32 unsigned int
//G B
//R G
class Bayer2RGB :public PixelConvertor
{
public:
	Bayer2RGB();
	~Bayer2RGB();
private:

public:

	virtual bool convert(unsigned char* dst, const unsigned char* src, int width, int height, unsigned char alpha = 0xff)const override;
	virtual bool point(const unsigned char *src, unsigned int &val, bool ret8BitsVal) const override
	{
		val = *src;
		return true;
	}
};

#endif // BAYER2RGB_H
