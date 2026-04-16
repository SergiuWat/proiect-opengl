#pragma once
#include <random>
#include <vector>
#include "Vector3.h"

struct TerrainVertex
{
    Vector3 position;
    Vector3 normal;
    float u;
    float v;
};

class Terrain
{
public:
    Terrain();

    void Generate(int width, int depth, float cellSize, float heightScale, float noiseScale, int seed);
    float GetHeightAt(int x, int z) const;
    void Render();
private:

    std::vector<TerrainVertex> vertices;
    std::vector<unsigned int> indices;

    int terrainWidth;
    int terrainDepth;
    float terrainCellSize;

    int p[512]; // permutation table

    void GeneratePermutation(int seed);

    float PerlinNoise(float x, float y) const;
    float Fade(float t) const;
    float Lerp(float a, float b, float t) const;
    float Grad(int hash, float x, float y) const;

    void CalculateNormals();
};

