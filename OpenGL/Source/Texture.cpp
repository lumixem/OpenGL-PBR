#include "Texture.h"
#include <glad/glad.h>
#include "FileManager.h"

Texture::Texture(FileManager* fileManager) :
fileManager(fileManager)
{
}

unsigned int Texture::CreateTexture(const char* path)
{
	//std::string filename = path;
	//std::string directory = "Resources/";
	//filename = directory + filename;
	std::string filename = "Resources/Textures/player_0.png";
	printf("Texture filepath: %s\n", filename.c_str());

	unsigned int texture;
	glGenTextures(1, &texture);

	int width, height, channels;
	unsigned char* data = fileManager->LoadImage(filename.c_str(), &width, &height, &channels);

















	if(data)
	{
		GLint format = {};
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;
		else
			printf("TEXTURE ERROR: Incorrect texture format\n");

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		fileManager->FreeImageData(data);
	}
	else
	{
		printf("TEXTURE ERROR: Texture failed to load\n");
		fileManager->FreeImageData(data);
	}

	return texture;
}