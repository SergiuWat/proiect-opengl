#pragma once
#include "Vector3.h"
#include <string>

enum class LightType
{
	Directional,
	Spotlight,
	Point
};

class Light
{
public:
	Light();

	std::string name;
	LightType type;

	bool active;
	bool visible;

	Vector3 position;
	Vector3 direction;

	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;

	float cutoff;
	float exponent;
};

