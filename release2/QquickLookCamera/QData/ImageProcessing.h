#pragma once


class ImageProcessing
{
private:
	int m_height;
	int m_width;
public:
	ImageProcessing(int height, int width);
	~ImageProcessing();

public:
	//24位真彩色图缩放，ratio为缩放因子（ratio=2即为放大2倍）
	void Zoom(unsigned char* dst, const unsigned char* src, int& width, int& height, double ratio);
	//24位真彩色图旋转angle°，图像高、宽改变，空白部分由白色填充
	void Rotate(unsigned char* dst, const unsigned char* src, int& width, int& height, int angle);
	//24位真彩色图转置，不同于旋转90°，高、宽对调
	void Transposition(unsigned char* dst, const unsigned char* src, int& width, int& height);
	//24位真彩色图平移，高、宽不变，（x,y）为顶点移动到的坐标，空白部分由白色填充
	bool Shift(unsigned char* dst, const unsigned char* src, int x, int y);
	//24位真彩色图模板运算（平滑、锐化）
	bool TemplateOperation(unsigned char* dst, const unsigned char* src, int templateType);
	//利用M3(Bayer抖动表),灰度图像转换为二值图像（此处仍由灰度图像表示，只是255表示白、0表示黑）
	bool LimbPatternM3(unsigned char* dst, const unsigned char* src);
};

