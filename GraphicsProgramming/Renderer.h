#pragma once
#include <vector>
#include "Light.h"

class GameObject;
class Transform;

class Renderer
{
public:
	Renderer();

	void Render(GameObject* gameObject);

	void DrawCube();
	void DrawSphere();
	void DrawModel(GameObject* gameObject);
	void ApplyTransform(Transform objectTransform);
	void ApplyMaterial(const GameObject* gameObject);
	void ApplyLights(const std::vector<Light*>& lights, const Vector3& globalAmbient);
	void ApplyTexture(GameObject* gameObject);
	void LoadTexture();

	void RenderSphereAtLightLocation(Light* light);
};

