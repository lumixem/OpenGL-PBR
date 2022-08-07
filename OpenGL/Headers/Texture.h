#pragma once
#include <string>

class FileManager;

class Texture
{
public:
	Texture(FileManager* fileManager);

	unsigned int CreateTexture(const char* path);

private:
	FileManager* fileManager;
};

