#include "TextureManager.h"
#include <iostream>
#include "SOIL.h"
TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	UnloadAllTextures();
}

GLuint TextureManager::LoadTexture(const std::string& filePath)
{
	auto it = textureCache.find(filePath);
	if (it != textureCache.end())
	{
		return it->second;
	}

	GLuint textureID = SOIL_load_OGL_texture(
		filePath.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	);

	if (textureID == 0)
	{
		std::cout << "Failed to load texture: " << filePath << std::endl;
		return 0;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	textureCache[filePath] = textureID;
	return textureID;
}

void TextureManager::UnloadAllTextures()
{
	for (auto& pair : textureCache)
	{
		glDeleteTextures(1, &pair.second);
	}
	textureCache.clear();
}
