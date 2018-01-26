#ifndef PIXEL8TO32_H
#define PIXEL8TO32_H

#include "PixelConvertor.h"

class Pixel8To32 : public PixelConvertor
{
public:
	Pixel8To32();
	~Pixel8To32();

	virtual bool convert(unsigned char* dst, const unsigned char* src, int width, int height, unsigned char alpha = 0xff) const override;
	virtual bool point(const unsigned char *src, unsigned int &val, bool ret8BitsVal) const override
	{
		val = *src;
		return true;
	}
};

#endif