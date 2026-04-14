#pragma once
#include "Vector3.h"
class Camera
{
public:
	Camera();

	void Update();

	float Yaw, Pitch, Roll;

	float speed = 10;
	float rotationSpeed = 90;
	void moveForward(float deltaTime);
	void moveBackward(float deltaTime);
	void moveRight(float deltaTime);
	void moveLeft(float deltaTime);

	void rotateLeft(float deltaTime);
	void rotateRight(float deltaTime);
	void rotateUp(float deltaTime);
	void rotateDown(float deltaTime);
private:
	Vector3 forward, up, right;
	Vector3 position;
	Vector3 lookAt;

public:

	Vector3 getForward();
	Vector3 getUp();
	Vector3 getPosition();
	Vector3 getLookAt();

	void setPosition(float x, float y, float z);
	void setPosition(Vector3 position);
	void setLookAt(float x, float y, float z);
	void setLookAt(Vector3 lookAt);
};

