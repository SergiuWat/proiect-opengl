#include "Transform.h"

Transform::Transform()
{
	position = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);
}

Transform::Transform(Vector3 position, Vector3 rotation, Vector3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

void Transform::setPosition(float x, float y, float z)
{
	position = Vector3(x, y, z);
}

void Transform::setPosition(Vector3 position)
{
	this->position = position;
}

void Transform::setRotation(float x, float y, float z)
{
	rotation = Vector3(x, y, z);
}

void Transform::setRotation(Vector3 rotation)
{
	this->rotation = rotation;
}

void Transform::setScale(float x, float y, float z)
{
	scale = Vector3(x, y, z);
}

void Transform::setScale(Vector3 scale)
{
	this->scale = scale;
}

Vector3 Transform::getPosition()
{
    return position;
}

Vector3 Transform::getRotation()
{
    return rotation;
}

Vector3 Transform::getScale()
{
    return scale;
}
