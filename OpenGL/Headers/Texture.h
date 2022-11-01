#pragma once
#include <string>

class FileManager;

class Texture
{
public:
	Texture(FileManager* fileManager);

	unsigned int CreateTexture(const char* path, const std::string& filepath);

private:
	FileManager* m_FileManager;
};