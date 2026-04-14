#include "GameObject.h"

GameObject::GameObject()
{
	id = 0;
	objectName = "GameObject";
	active = true;
	visible = true;
	selected = false;

	type = GameObjectType::Primitive;
	primitiveType = PrimitiveType::Cube;

	modelPath = "";
	modelTexturePath = "";

	textureID = 0;
	model = nullptr;
}

GameObject::~GameObject()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}
