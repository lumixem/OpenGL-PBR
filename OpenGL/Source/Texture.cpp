#include "Texture.h"
#include "FileManager.h"
#include <glad/glad.h>

Texture::Texture(FileManager* fileManager) :
m_FileManager(fileManager)
{
}

unsigned int Texture::CreateTexture(const char* textureName, const std::string& filepath)
{
	std::string filename = textureName;
	filename = filepath + filename;

	unsigned int texture;
	glGenTextures(1, &texture);

	int width, height, channels;
	unsigned char* data = m_FileManager->LoadImage(filename.c_str(), &width, &height, &channels);

	if(data)
	{
		GLint format = {};
		std::string strFormat;
		if (m_SrgbFormat)
		{
			format = GL_SRGB;
			strFormat = "SRGB";
		}
		else if (channels == 1)
		{
			format = GL_RED;
			strFormat = "RED";
		}
		else if (channels == 3)
		{
			format = GL_RGB;
			strFormat = "RGB";
		}
		else if (channels == 4)
		{
			format = GL_RGBA;
			strFormat = "RGBA";
		}
		else
			printf("\nTEXTURE ERROR: Incorrect texture format\n");

		printf("Texture filepath: %s, format: %s\n", filename.c_str(), strFormat.c_str());
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		m_FileManager->FreeImageData(data);
	}
	else
	{
		printf("\nTEXTURE ERROR: Texture failed to load\n");
		m_FileManager->FreeImageData(data);
	}

	return texture;
}