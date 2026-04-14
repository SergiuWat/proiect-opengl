#include "Camera.h"
#include <math.h>

Camera::Camera()
{
	position = Vector3(0, 0, 6);
	lookAt = Vector3(0, 0, 0);
	up = Vector3(0, 1, 0);
}

void Camera::Update()
{
	float cosR, cosP, cosY; //temp values for sin/cos from
	float sinR, sinP, sinY;
	// Roll, Pitch and Yall are variables stored by the camera
	// handle rotation
	// Only want to calculate these values once, when rotation changes, not every frame.
	cosY = cosf(Yaw * 3.1415 / 180);
	cosP = cosf(Pitch * 3.1415 / 180);
	cosR = cosf(Roll * 3.1415 / 180);
	sinY = sinf(Yaw * 3.1415 / 180);
	sinP = sinf(Pitch * 3.1415 / 180);
	sinR = sinf(Roll * 3.1415 / 180);

	forward.x = sinY * cosP;
	forward.y = sinP;
	forward.z = cosP * -cosY;
	
	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP * cosR * -cosY;

	right = forward.cross(up);

	lookAt = position + forward;
}

void Camera::moveForward(float deltaTime)
{
	position.x += forward.x * speed * deltaTime;
	position.y += forward.y * speed * deltaTime;
	position.z += forward.z * speed * deltaTime;
}

void Camera::moveBackward(float deltaTime)
{
	position.x -= forward.x * speed * deltaTime;
	position.y -= forward.y * speed * deltaTime;
	position.z -= forward.z * speed * deltaTime;
}

void Camera::moveRight(float deltaTime)
{
	position.x += right.x * speed * deltaTime;
	position.y += right.y * speed * deltaTime;
	position.z += right.z * speed * deltaTime;
}

void Camera::moveLeft(float deltaTime)
{
	position.x -= right.x * speed * deltaTime;
	position.y -= right.y * speed * deltaTime;
	position.z -= right.z * speed * deltaTime;
}

void Camera::rotateLeft(float deltaTime)
{
	Yaw -= rotationSpeed * deltaTime;
}

void Camera::rotateRight(float deltaTime)
{
	Yaw += rotationSpeed * deltaTime;
}

void Camera::rotateUp(float deltaTime)
{
	Pitch -= rotationSpeed * deltaTime;
}

void Camera::rotateDown(float deltaTime)
{
	Pitch += rotationSpeed * deltaTime;
}

Vector3 Camera::getForward()
{
	return forward;
}

Vector3 Camera::getUp()
{
	return up;
}

Vector3 Camera::getPosition()
{
	return position;
}

Vector3 Camera::getLookAt()
{
	return lookAt;
}

void Camera::setPosition(float x, float y, float z)
{
	position = Vector3(x, y, z);
}

void Camera::setPosition(Vector3 position)
{
	this->position = position;
}

void Camera::setLookAt(float x, float y, float z)
{
	lookAt = Vector3(x, y, z);
}

void Camera::setLookAt(Vector3 lookAt)
{
	this->lookAt = lookAt;
}
