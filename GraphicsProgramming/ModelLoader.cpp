#include "ModelLoader.h"
#include <cstdio>
#include <print>
#include <iostream>

ModelLoader::ModelLoader()
{

}

bool ModelLoader::LoadModel(const char* filePath)
{
	vertices.clear();
	normals.clear();
	texCoords.clear();
	faces.clear();

	FILE* file = fopen(filePath, "r");
	if (file == NULL)
	{
		printf("Could not open file: %s\n", filePath);
		return false;
	}
	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break;
		}
		else {
			if (strcmp(lineHeader, "v") == 0) // Vertex
			{

				Vector3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coordx
			{

				Vector3 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				texCoords.push_back(uv);

			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				Vector3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[9];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&face[0], &face[1], &face[2],
					&face[3], &face[4], &face[5],
					&face[6], &face[7], &face[8]);

				if (matches != 9)
				{

					// Parser error, or not triangle faces
					fclose(file);
					return false;

				}
				for (int i = 0; i < 9; i++)
				{
					this->faces.push_back(face[i]);
				}
			}
		}	
	}
	fclose(file);
	return true;
}
//
//bool ModelLoader::LoadTexture(const char* filePath)
//{
//	if (texture != 0)
//	{
//		glDeleteTextures(1, &texture);
//		texture = 0;
//	}
//
//	texture = SOIL_load_OGL_texture(
//		filePath,
//		SOIL_LOAD_AUTO,
//		SOIL_CREATE_NEW_ID,
//		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
//	);
//
//	if (texture == 0)
//	{
//		std::cout << "Failed to load texture: " << filePath << std::endl;
//		return false;
//	}
//
//	glBindTexture(GL_TEXTURE_2D, texture);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	return true;
//}
//
//bool ModelLoader::LoadObject(const char* filePathModel, const char* filePathTexture)
//{
//	bool modelLoaded = LoadModel(filePathModel);
//	if (!modelLoaded)
//		return false;
//
//	bool textureLoaded = LoadTexture(filePathTexture);
//	if (!textureLoaded)
//		return false;
//
//	return true;
//}

void ModelLoader::RenderModel()
{
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	//if (texture != 0)
	//{
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//}
	//else
	//{
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(4.0f, 4.0f, 4.0f);

	for (int i = 0; i < faces.size(); i += 9)
	{
		glBegin(GL_TRIANGLES);
		std::cout << "x: " << texCoords[faces[i + 1] - 1].x << " y: " << texCoords[faces[i + 1] - 1].y << std::endl;
		glTexCoord2f(texCoords[faces[i + 1] - 1].x, texCoords[faces[i + 1] - 1].y);
		glVertex3f(vertices[faces[i]-1].x, vertices[faces[i]-1].y, vertices[faces[i]-1].z);
		glTexCoord2f(texCoords[faces[i + 4] - 1].x, texCoords[faces[i + 4] - 1].y);
		glVertex3f(vertices[faces[i + 3]-1].x, vertices[faces[i + 3]-1].y, vertices[faces[i + 3]-1].z);
		glTexCoord2f(texCoords[faces[i + 7] - 1].x, texCoords[faces[i + 7] - 1].y);
		glVertex3f(vertices[faces[i + 6]-1].x, vertices[faces[i + 6]-1].y, vertices[faces[i + 6]-1].z);
		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

unsigned int ModelLoader::GetTextureID() const
{
	return texture;
}