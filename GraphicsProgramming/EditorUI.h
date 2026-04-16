#pragma once
#include <string>
#include <filesystem>

class Scene;
class GameObject;
enum class PrimitiveType;

namespace fs = std::filesystem;

class EditorUI
{
public:

	EditorUI(Scene* ownerScene);

	void Render();

private:

	Scene* scene;

	void DeveloperInformation();
	void ContentBrowserUI();
	void PrimitiveObjectLoaderWindow();
	void SceneHierarchyUI();
	void ObjectDetailsUI();
	void TerrainUI();
	const char* GetGameObjectDisplayName(GameObject* gameObject, int index);

	std::string selectedModelPath;
	std::string selectedTexturePath;
};

