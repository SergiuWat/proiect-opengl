#pragma once
#include "Transform.h"
#include <iostream>
#include "GameObjectTypes.h"

class ModelLoader;
class GameObject
{
public:

	GameObject();
	~GameObject();
	int id;
	std::string objectName;
	bool active;
	bool visible;
	bool selected;

	GameObjectType type;
	PrimitiveType primitiveType;

	std::string modelPath;
	std::string modelTexturePath;
	unsigned int textureID;
	ModelLoader* model;
	Transform transform;
};

