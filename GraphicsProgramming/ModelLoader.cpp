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

void ModelLoader::RenderModel()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(4.0f, 4.0f, 4.0f);

	for (int i = 0; i < faces.size(); i += 9)
	{
		glBegin(GL_TRIANGLES);

		unsigned int v1 = faces[i] - 1;
		unsigned int t1 = faces[i + 1] - 1;
		unsigned int n1 = faces[i + 2] - 1;

		unsigned int v2 = faces[i + 3] - 1;
		unsigned int t2 = faces[i + 4] - 1;
		unsigned int n2 = faces[i + 5] - 1;

		unsigned int v3 = faces[i + 6] - 1;
		unsigned int t3 = faces[i + 7] - 1;
		unsigned int n3 = faces[i + 8] - 1;

		if (!normals.empty())
			glNormal3f(normals[n1 * 3], normals[n1 * 3 + 1], normals[n1 * 3 + 2]);
		if (!texCoords.empty())
			glTexCoord2f(texCoords[t1].x, texCoords[t1].y);
		glVertex3f(vertices[v1].x, vertices[v1].y, vertices[v1].z);

		if (!normals.empty())
			glNormal3f(normals[n2 * 3], normals[n2 * 3 + 1], normals[n2 * 3 + 2]);
		if (!texCoords.empty())
			glTexCoord2f(texCoords[t2].x, texCoords[t2].y);
		glVertex3f(vertices[v2].x, vertices[v2].y, vertices[v2].z);

		if (!normals.empty())
			glNormal3f(normals[n3 * 3], normals[n3 * 3 + 1], normals[n3 * 3 + 2]);
		if (!texCoords.empty())
			glTexCoord2f(texCoords[t3].x, texCoords[t3].y);
		glVertex3f(vertices[v3].x, vertices[v3].y, vertices[v3].z);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int ModelLoader::GetTextureID() const
{
	return texture;
}