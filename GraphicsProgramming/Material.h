#pragma once
#include "Vector3.h"

class Material
{
public:
	Material();

	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	float shininess;
};

