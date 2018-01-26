#include "Pixel8To32.h"


Pixel8To32::Pixel8To32()
{
}


Pixel8To32::~Pixel8To32()
{
}

bool Pixel8To32::convert(unsigned char* dst, const unsigned char* src, int width, int height, unsigned char alpha) const
{
	int v;
	for (int h = 0; h < height; ++h)
	{
		for (int w = 0; w < width; ++w)
		{
			v = *(src + h*width + w);
			*(((unsigned int*)dst) + h*width + w) = (v | (v << 16) | (v << 8) | (alpha << 24));
		}
	}
	return true;
}