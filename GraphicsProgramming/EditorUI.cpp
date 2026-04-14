#include "EditorUI.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include "Camera.h"

EditorUI::EditorUI(Scene* ownerScene)
{
	scene = ownerScene;
}

void EditorUI::Render()
{
	if (!scene->editorMode)
		return;

	PrimitiveObjectLoaderWindow();
	DeveloperInformation();
	ContentBrowserUI();
	SceneHierarchyUI();
}

void EditorUI::DeveloperInformation()
{
	ImVec2 windowSize = ImVec2(260, 220);
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImVec2 windowPos = ImVec2(displaySize.x - windowSize.x - 20.0f, 100.0f);

	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
	ImGui::Begin("Developers Information");

	ImGui::Text("Press 'l' for toggle mouse");
	ImGui::Separator();
	ImGui::Text("Mouse Locked: %s", scene->mouseLocked ? "ON" : "OFF");

	ImGui::Text("Press 'h' to toggle to editor");
	ImGui::Text("Editor Mode: %s", scene->editorMode ? "ON" : "OFF");

	ImGui::Separator();
	ImGui::Text("Press 'backspace' to delete last object");
	ImGui::Separator();

	ImGui::Text("Snap Mode: %s", scene->snapEnabled ? "ON" : "OFF");
	ImGui::Text("Press 'j' to toggle snap");
	ImGui::Separator();

	ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)",
		scene->camera.getPosition().x,
		scene->camera.getPosition().y,
		scene->camera.getPosition().z);

	ImGui::End();
}

void EditorUI::ContentBrowserUI()
{
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	float windowHeight = 300.0f;
	float windowWidth = displaySize.x / 3.0f;
	float startY = displaySize.y - windowHeight;

	// Models
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0.0f, startY), ImGuiCond_Always);
	ImGui::Begin("3D Objects");

	ImGui::Text("Folder: models/");
	ImGui::Separator();

	if (fs::exists("models/") && fs::is_directory("models/"))
	{
		for (const auto& entry : fs::directory_iterator("models/"))
		{
			if (!entry.is_regular_file())
				continue;

			std::string extension = entry.path().extension().string();
			if (extension == ".obj")
			{
				std::string fullPath = entry.path().string();
				std::string fileName = entry.path().filename().string();

				bool isSelected = (selectedModelPath == fullPath);
				if (ImGui::Selectable(fileName.c_str(), isSelected))
				{
					selectedModelPath = fullPath;
				}
			}
		}
	}
	else
	{
		ImGui::Text("models/ folder not found.");
	}

	ImGui::Separator();
	ImGui::TextWrapped("Selected Model: %s", selectedModelPath.empty() ? "None" : selectedModelPath.c_str());
	ImGui::End();

	// Textures
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(windowWidth, startY), ImGuiCond_Always);
	ImGui::Begin("Textures");

	ImGui::Text("Folder: textures/");
	ImGui::Separator();

	if (fs::exists("textures/") && fs::is_directory("textures/"))
	{
		for (const auto& entry : fs::directory_iterator("textures/"))
		{
			if (!entry.is_regular_file())
				continue;

			std::string extension = entry.path().extension().string();

			if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp" || extension == ".tga")
			{
				std::string fullPath = entry.path().string();
				std::string fileName = entry.path().filename().string();

				bool isSelected = (selectedTexturePath == fullPath);
				if (ImGui::Selectable(fileName.c_str(), isSelected))
				{
					selectedTexturePath = fullPath;
				}
			}
		}
	}
	else
	{
		ImGui::Text("textures/ folder not found.");
	}

	ImGui::Separator();
	ImGui::TextWrapped("Selected Texture: %s", selectedTexturePath.empty() ? "None" : selectedTexturePath.c_str());
	ImGui::End();

	// Loader
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(windowWidth * 2.0f, startY), ImGuiCond_Always);
	ImGui::Begin("Asset Loader");

	ImGui::TextWrapped("Model: %s", selectedModelPath.empty() ? "None" : selectedModelPath.c_str());
	ImGui::TextWrapped("Texture: %s", selectedTexturePath.empty() ? "None" : selectedTexturePath.c_str());

	if (ImGui::Button("Load Object"))
	{
		if (!selectedModelPath.empty() && !selectedTexturePath.empty())
		{
			scene->selectedGameObject = scene->objectLoad(selectedModelPath, selectedTexturePath);
		}
	}

	ImGui::End();
}

void EditorUI::PrimitiveObjectLoaderWindow()
{
	ImGui::SetNextWindowSize(ImVec2(260, 220), ImGuiCond_FirstUseEver);
	ImGui::Begin("Editor");

	ImGui::Text("Developer Mode: ON");
	ImGui::Separator();

	if (ImGui::Button("Add Cube"))
	{
		scene->createPrimitivePreview(PrimitiveType::Cube);
	}

	if (ImGui::Button("Add Sphere"))
	{
		scene->createPrimitivePreview(PrimitiveType::Sphere);
	}

	ImGui::Separator();

	if (scene->selectedGameObject != nullptr)
	{
		ImGui::Text("Preview Object: %s", scene->selectedGameObject->objectName.c_str());
		ImGui::Text("Right click to place object");
	}
	else
	{
		ImGui::Text("No preview object selected");
	}

	ImGui::Text("Placed Objects: %d", (int)scene->gameObjects.size());

	ImGui::End();
}

void EditorUI::SceneHierarchyUI()
{
	float windowHeight = 300.0f;
	float windowWidth = 300.0f;

	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0.0f, 300.0f), ImGuiCond_Always);

	ImGui::Begin("Scene Hierarchy");

	if (scene->gameObjects.empty())
	{
		ImGui::Text("No objects in scene.");
	}
	else
	{
		for (int i = 0; i < (int)scene->gameObjects.size(); i++)
		{
			GameObject* gameObject = scene->gameObjects[i];
			if (gameObject == nullptr)
				continue;

			bool isSelected = (scene->inspectedGameObject == gameObject);

			if (ImGui::Selectable(GetGameObjectDisplayName(gameObject, i), isSelected))
			{
				scene->inspectedGameObject = gameObject;
			}
		}
	}

	ImGui::End();
}

const char* EditorUI::GetGameObjectDisplayName(GameObject* gameObject, int index)
{
	static std::string label;

	if (gameObject == nullptr)
	{
		label = "Null Object";
		return label.c_str();
	}

	label = gameObject->objectName + " " + std::to_string(index);
	return label.c_str();
}