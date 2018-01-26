#include "SaveToBmp24.h"

SaveToBmp24::SaveToBmp24()
{
}

SaveToBmp24::~SaveToBmp24()
{
}

void SaveToBmp24::save(const unsigned char* src, int height, int width, std::string path, std::string name)
{
	int imagDataSize = height*width * 3; // imag data size

	//位图第一部分，位图文件头
	BITMAPFILEHEADER *bfHeader{ new BITMAPFILEHEADER };
	bfHeader->bfType = 0x4d42;  // string"BM"  
	bfHeader->bfSize = imagDataSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // file size
	bfHeader->bfReserved1 = 0; // reserved  
	bfHeader->bfReserved2 = 0; // reserved  
	bfHeader->bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); // real data 位置  

	//位图第二部分，位图信息头  
	BITMAPINFOHEADER *biHeader{ new BITMAPINFOHEADER };
	biHeader->biSize = sizeof(BITMAPINFOHEADER);
	biHeader->biWidth = width;
	biHeader->biHeight = height;//BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了  
	biHeader->biPlanes = 1;//为1，不用改  
	biHeader->biBitCount = 24; // 每个像素占用的bit
	biHeader->biCompression = BI_RGB;//不压缩  
	biHeader->biSizeImage = imagDataSize;
	biHeader->biXPelsPerMeter = 0;//像素每米  
	biHeader->biYPelsPerMeter = 0;
	biHeader->biClrUsed = 0;//已用过的颜色，24位的为0  
	biHeader->biClrImportant = 0;//每个像素都重要 

	//打开文件并保存
	//文件路径
	std::string fpath;
	fpath += path;
	if (name.empty())
		fpath += "IMG_";
	else
		fpath += name + "_";
	SYSTEMTIME st;
	GetLocalTime(&st);
	char time[20];
	sprintf_s(time, sizeof(st), "%4d%2d%2d_%2d%2d%2d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	for (int i = 0; time[i]; ++i)
	{
		if (time[i] == ' ') time[i] = '0';
	}
	fpath += time;
	fpath += ".bmp";
	std::fstream file(fpath, std::ios::out | std::ios::binary);
	file.write((char*)bfHeader, sizeof(BITMAPFILEHEADER));
	file.write((char*)biHeader, sizeof(BITMAPINFOHEADER));
	file.write((char*)src, imagDataSize);
	file.close();
}