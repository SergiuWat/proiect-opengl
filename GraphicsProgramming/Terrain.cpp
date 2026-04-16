#include "Terrain.h"
#include <cmath>
#include <numeric>
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


Terrain::Terrain()
{
    terrainWidth = 0;
    terrainDepth = 0;
    terrainCellSize = 1.0f;
}

void Terrain::Generate(int width, int depth, float cellSize, float heightScale, float noiseScale, int seed)
{
    GeneratePermutation(seed); 

    terrainWidth = width;
    terrainDepth = depth;
    terrainCellSize = cellSize;

    vertices.clear();
    indices.clear();

    for (int z = 0; z <= depth; z++)
    {
        for (int x = 0; x <= width; x++)
        {
            float worldX = x * cellSize;
            float worldZ = z * cellSize;

            float noise = PerlinNoise(x * noiseScale, z * noiseScale);
            float height = noise * heightScale;

            TerrainVertex vertex;
            vertex.position = Vector3(worldX, height, worldZ);
            vertex.normal = Vector3(0, 1, 0);

            float tile = 10.0f;

            vertex.u = (float)x / width * tile;
            vertex.v = (float)z / depth * tile;

            vertices.push_back(vertex);
        }
    }

    for (int z = 0; z < depth; z++)
    {
        for (int x = 0; x < width; x++)
        {
            int topLeft = z * (width + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (width + 1) + x;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    CalculateNormals();
}

float Terrain::GetHeightAt(int x, int z) const
{
    if (x < 0 || z < 0 || x > terrainWidth || z > terrainDepth)
        return 0.0f;

    int index = z * (terrainWidth + 1) + x;
    return vertices[index].position.y;
}

void Terrain::Render()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLES);

    for (size_t i = 0; i < indices.size(); i++)
    {
        const TerrainVertex& v = vertices[indices[i]];

        glNormal3f(v.normal.x, v.normal.y, v.normal.z);
        glTexCoord2f(v.u, v.v);
        glVertex3f(v.position.x, v.position.y, v.position.z);
    }

    glEnd();
}

void Terrain::GeneratePermutation(int seed)
{
    std::vector<int> permutation(256);

    for (int i = 0; i < 256; i++)
        permutation[i] = i;

    std::mt19937 rng(seed);
    std::shuffle(permutation.begin(), permutation.end(), rng);

    for (int i = 0; i < 256; i++)
    {
        p[i] = permutation[i];
        p[256 + i] = permutation[i];
    }
}

float Terrain::PerlinNoise(float x, float y) const
{
    int xi = (int)std::floor(x) & 255;
    int yi = (int)std::floor(y) & 255;

    float xf = x - std::floor(x);
    float yf = y - std::floor(y);

    float u = Fade(xf);
    float v = Fade(yf);

    int aa = p[p[xi] + yi];
    int ab = p[p[xi] + yi + 1];
    int ba = p[p[xi + 1] + yi];
    int bb = p[p[xi + 1] + yi + 1];

    float x1 = Lerp(Grad(aa, xf, yf), Grad(ba, xf - 1, yf), u);
    float x2 = Lerp(Grad(ab, xf, yf - 1), Grad(bb, xf - 1, yf - 1), u);

    return (Lerp(x1, x2, v) + 1.0f) * 0.5f;
}

float Terrain::Fade(float t) const
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float Terrain::Lerp(float a, float b, float t) const
{
    return a + t * (b - a);
}

float Terrain::Grad(int hash, float x, float y) const
{
    switch (hash & 3)
    {
    case 0: return x + y;
    case 1: return -x + y;
    case 2: return x - y;
    case 3: return -x - y;
    default: return 0.0f;
    }
}

void Terrain::CalculateNormals()
{
    for (auto& v : vertices)
    {
        v.normal = Vector3(0.0f, 0.0f, 0.0f);
    }

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        TerrainVertex& v0 = vertices[indices[i]];
        TerrainVertex& v1 = vertices[indices[i + 1]];
        TerrainVertex& v2 = vertices[indices[i + 2]];

        Vector3 edge1(
            v1.position.x - v0.position.x,
            v1.position.y - v0.position.y,
            v1.position.z - v0.position.z
        );

        Vector3 edge2(
            v2.position.x - v0.position.x,
            v2.position.y - v0.position.y,
            v2.position.z - v0.position.z
        );

        Vector3 normal(
            edge1.y * edge2.z - edge1.z * edge2.y,
            edge1.z * edge2.x - edge1.x * edge2.z,
            edge1.x * edge2.y - edge1.y * edge2.x
        );

        v0.normal.x += normal.x; v0.normal.y += normal.y; v0.normal.z += normal.z;
        v1.normal.x += normal.x; v1.normal.y += normal.y; v1.normal.z += normal.z;
        v2.normal.x += normal.x; v2.normal.y += normal.y; v2.normal.z += normal.z;
    }

    for (auto& v : vertices)
    {
        float len = std::sqrt(v.normal.x * v.normal.x + v.normal.y * v.normal.y + v.normal.z * v.normal.z);
        if (len > 0.0001f)
        {
            v.normal.x /= len;
            v.normal.y /= len;
            v.normal.z /= len;
        }
        else
        {
            v.normal = Vector3(0.0f, 1.0f, 0.0f);
        }
    }
}
