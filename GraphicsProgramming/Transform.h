#pragma once
#include "Vector3.h"

class Transform
{
public:

	Transform();
	Transform(Vector3 position, Vector3 rotation, Vector3 scale);

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	void setPosition(float x, float y, float z);
	void setPosition(Vector3 position);
	void setRotation(float x, float y, float z);
	void setRotation(Vector3 rotation);
	void setScale(float x, float y, float z);
	void setScale(Vector3 scale);

	Vector3 getPosition();
	Vector3 getRotation();
	Vector3 getScale();
};

