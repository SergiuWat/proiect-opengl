#pragma once
#include <vector>
#include "Vector3.h"
#include "SOIL.h"
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
class ModelLoader
{
public:
	ModelLoader();
	bool LoadModel(const char* filePath);
	//bool LoadTexture(const char* filePath);
	//bool LoadObject(const char* filePathModel, const char* filePathTexture);
	void RenderModel();

	unsigned int GetTextureID() const;
private:
	GLuint texture;
	std::vector<Vector3> vertices;
	std::vector<Vector3> texCoords;
	std::vector<float> normals;
	std::vector<unsigned int> faces;
};

