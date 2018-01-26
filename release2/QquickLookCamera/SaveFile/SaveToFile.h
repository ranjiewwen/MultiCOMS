#ifndef _SAVE_TO_FILE_H_
#define _SAVE_TO_FILE_H_

#include <windows.h>
#include <fstream>
#include <string>
#include <time.h>
#include<qstring.h>
class SaveToFile
{
public:
	SaveToFile();
	virtual ~SaveToFile();
	/*
	//将图像数据保存为文件，由给定命名+系统时间命名，默认为“IMG_YYYYMMDD_HHmmSS.bmp”
	//path：即路径，格式示例"C:\\Users\\Administrator\\Desktop\\QView\\"   //由于 \ 也是转义字符的起始字符
	*/
	virtual void save(const unsigned char* src, int height, int width, std::string path, std::string name = "") = 0;
};

#endif