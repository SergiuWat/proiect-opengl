#pragma once

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
	void ApplyMaterial();
	void ApplyTexture(GameObject* gameObject);
	void LoadTexture();

};

