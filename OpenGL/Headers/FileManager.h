#pragma once
#include <string>

class FileManager
{
public:
	std::string LoadText(const char* filename);
	unsigned char* LoadImage(const char* filename, int* width, int* height, int* channels);
	void FreeImageData(unsigned char* data);
};