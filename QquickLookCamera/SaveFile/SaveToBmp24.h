#ifndef _SAVE_TO_BMP24_H_
#define _SAVE_TO_BMP24_H_

#include "SaveToFile.h"

class SaveToBmp24:public SaveToFile
{
public:
	SaveToBmp24();
	~SaveToBmp24();
	bool save(const unsigned char* src, int height, int width, std::string path, std::string name = "") override;
};

#endif