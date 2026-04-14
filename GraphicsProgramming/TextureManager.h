#pragma once
#include <string>
#include <unordered_map>
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	GLuint LoadTexture(const std::string& filePath);

	void UnloadAllTextures();

private:
	std::unordered_map<std::string, GLuint> textureCache;
};

