#ifndef _SAVE_TO_BMPGRAY_H_
#define _SAVE_TO_BMPGRAY_H_

#include "SaveToFile.h"

class SaveToBmpGray :public SaveToFile
{
public:
	SaveToBmpGray();
	~SaveToBmpGray();
	void save(const unsigned char* src, int height, int width, std::string path, std::string name = "") override;
};

#endif