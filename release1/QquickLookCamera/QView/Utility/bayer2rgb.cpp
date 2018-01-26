#include "bayer2rgb.h"


Bayer2RGB::Bayer2RGB()
{
	
}
Bayer2RGB::~Bayer2RGB()
{

}

void bayer_copy(unsigned char* dst, const unsigned char* src, int width, int height, int x, int y)
{
	G(x + 0, y + 0) = Bay(x + 0, y + 0);
	G(x + 1, y + 1) = Bay(x + 1, y + 1);
	G(x + 0, y + 1) = G(x + 1, y + 0) = ((u32)Bay(x + 0, y + 0) + (u32)Bay(x + 1, y + 1)) / 2;

	R(x + 0, y + 0) = R(x + 1, y + 0) = R(x + 1, y + 1) = R(x + 0, y + 1) = Bay(x + 0, y + 1);
	B(x + 1, y + 1) = B(x + 0, y + 0) = B(x + 0, y + 1) = B(x + 1, y + 0) = Bay(x + 1, y + 0);
}

//中间像素处理
void bayer_bilinear(unsigned char* dst, const unsigned char* src, int width, int height, int x, int y)
{
	R(x + 0, y + 0) = ((u32)Bay(x + 0, y + 1) + (u32)Bay(x + 0, y - 1)) / 2;
	G(x + 0, y + 0) = Bay(x + 0, y + 0);
	B(x + 0, y + 0) = ((u32)Bay(x - 1, y + 0) + (u32)Bay(x + 1, y + 0)) / 2;

	R(x + 0, y + 1) = Bay(x + 0, y + 1);
	G(x + 0, y + 1) = ((u32)Bay(x + 0, y + 0) + (u32)Bay(x + 0, y + 2) + (u32)Bay(x - 1, y + 1) + (u32)Bay(x + 1, y + 1)) / 4;
	B(x + 0, y + 1) = ((u32)Bay(x + 1, y + 0) + (u32)Bay(x - 1, y + 0) + (u32)Bay(x + 1, y + 2) + (u32)Bay(x - 1, y + 2)) / 4;

	R(x + 1, y + 0) = ((u32)Bay(x + 0, y + 1) + (u32)Bay(x + 2, y + 1) + (u32)Bay(x + 0, y - 1) + (u32)Bay(x + 2, y - 1)) / 4;
	G(x + 1, y + 0) = ((u32)Bay(x + 0, y + 0) + (u32)Bay(x + 2, y + 0) + (u32)Bay(x + 1, y - 1) + (u32)Bay(x + 1, y + 1)) / 4;
	B(x + 1, y + 0) = Bay(x + 1, y + 0);

	R(x + 1, y + 1) = ((u32)Bay(x + 0, y + 1) + (u32)Bay(x + 2, y + 1)) / 2;
	G(x + 1, y + 1) = Bay(x + 1, y + 1);
	B(x + 1, y + 1) = ((u32)Bay(x + 1, y + 0) + (u32)Bay(x + 1, y + 2)) / 2;
}
//void bayer_copy(unsigned char* dst, const unsigned char* src, int width, int height, int x, int y)
//{
//    G(x + 0, y + 1) = Bay(x + 0, y + 1);
//    G(x + 1, y + 0) = Bay(x + 1, y + 0);
//	G(x + 0, y + 0) = G(x + 1, y + 1) = ((u32)Bay(x + 0, y + 1) + (u32)Bay(x + 1, y + 0)) / 2;
//
//    R(x + 0, y + 0) = R(x + 1, y + 0) = R(x + 1, y + 1) = R(x + 0, y + 1) = Bay(x + 0, y + 0);
//    B(x + 1, y + 1) = B(x + 0, y + 0) = B(x + 0, y + 1) = B(x + 1, y + 0) = Bay(x + 1, y + 1);
//}
//
////中间像素处理
//void bayer_bilinear(unsigned char* dst, const unsigned char* src, int width, int height, int x, int y)
//{
//	R(x + 0, y + 0) = Bay(x + 0, y + 0);
//	G(x + 0, y + 0) = ((u32)Bay(x - 1, y + 0) + (u32)Bay(x + 1, y + 0) + (u32)Bay(x + 0, y - 1) + (u32)Bay(x + 0, y + 1)) / 4;
//	B(x + 0, y + 0) = ((u32)Bay(x - 1, y - 1) + (u32)Bay(x - 1, y + 1) + (u32)Bay(x + 1, y - 1) + (u32)Bay(x + 1, y + 1)) / 4;
//	
//	R(x + 0, y + 1) = ((u32)Bay(x + 0, y + 0) + (u32)Bay(x + 0, y + 2)) / 2;
//    G(x + 0, y + 1) = Bay(x + 0, y + 1);
//	B(x + 0, y + 1) = ((u32)Bay(x - 1, y + 1) + (u32)Bay(x + 1, y + 1)) / 2;
//
//	R(x + 1, y + 0) = ((u32)Bay(x + 0, y + 0) + (u32)Bay(x + 2, y + 0)) / 2;
//	G(x + 1, y + 0) = Bay(x + 1, y + 0);
//	B(x + 1, y + 0) = ((u32)Bay(x + 1, y - 1) + (u32)Bay(x + 1, y + 1)) / 2;
//
//	R(x + 1, y + 0) = ((u32)Bay(x + 0, y + 0) + (u32)Bay(x + 0, y + 2) + (u32)Bay(x + 2, y + 0) + (u32)Bay(x + 2, y + 2)) / 4;
//	G(x + 1, y + 0) = ((u32)Bay(x + 0, y + 0) + (u32)Bay(x + 2, y + 0) + (u32)Bay(x + 1, y - 1) + (u32)Bay(x + 1, y + 1)) / 4;
//    B(x + 1, y + 0) = Bay(x + 1, y + 1);
//}
	
bool Bayer2RGB::convert(unsigned char* dst, const unsigned char* src, int width, int height, unsigned char alpha) const
{
	int i, j;

	for (i = 0; i < width; i += 2)
	{
		for (j = 0; j < height; j += 2)
		{
			if (i == 0 || j == 0 || i == width - 2 || j == height - 2)
			{
				bayer_copy(dst,src,width,height,i, j);
			}
			else
			{
				bayer_bilinear(dst, src, width, height, i, j);
			}
		}
	}
	return true;
}

