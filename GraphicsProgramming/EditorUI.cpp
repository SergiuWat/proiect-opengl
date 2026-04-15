#include "EditorUI.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include "Camera.h"
#include <cstring>
#include "Light.h"
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
	ObjectDetailsUI();
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


	ImGui::Separator();
	ImGui::Text("Lights");

	if (ImGui::Button("Add Directional Light"))
	{
		scene->createLightPreview(LightType::Directional);
	}

	if (ImGui::Button("Add Spot Light"))
	{
		scene->createLightPreview(LightType::Spotlight);
	}

	if (ImGui::Button("Add Point Light"))
	{
		scene->createLightPreview(LightType::Point);
	}

	if (scene->selectedLight != nullptr)
	{
		ImGui::Text("Preview Light: %s", scene->selectedLight->name.c_str());
		ImGui::Text("Right click to place light");
	}

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
				scene->inspectedLight = nullptr;
			}
		}
	}

	ImGui::Separator();
	ImGui::Text("Lights");

	for (int i = 0; i < (int)scene->lights.size(); i++)
	{
		Light* light = scene->lights[i];
		if (light == nullptr)
			continue;

		bool isSelected = (scene->inspectedLight == light);

		std::string label = light->name + " " + std::to_string(i);
		if (ImGui::Selectable(label.c_str(), isSelected))
		{
			scene->inspectedLight = light;
			scene->inspectedGameObject = nullptr;
		}
	}

	ImGui::End();
}

void EditorUI::ObjectDetailsUI()
{
	float windowWidth = 340.0f;
	float windowHeight = 650.0f;

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(displaySize.x - windowWidth - 20.0f, 340.0f), ImGuiCond_Always);

	ImGui::Begin("Inspector");

	// =========================
	// LIGHT INSPECTOR
	// =========================
	if (scene->inspectedLight != nullptr)
	{
		Light* light = scene->inspectedLight;

		ImGui::Text("Light: %s", light->name.c_str());
		ImGui::Separator();

		char nameBuffer[256];
		strcpy_s(nameBuffer, sizeof(nameBuffer), light->name.c_str());
		if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
		{
			light->name = nameBuffer;
		}

		const char* lightTypes[] = { "Directional", "Spotlight", "Point" };
		int currentType = 0;

		switch (light->type)
		{
		case LightType::Directional:
			currentType = 0;
			break;
		case LightType::Spotlight:
			currentType = 1;
			break;
		case LightType::Point:
			currentType = 2;
			break;
		}

		if (ImGui::Combo("Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes)))
		{
			switch (currentType)
			{
			case 0:
				light->type = LightType::Directional;
				break;
			case 1:
				light->type = LightType::Spotlight;
				break;
			case 2:
				light->type = LightType::Point;
				break;
			}
		}

		float position[3] =
		{
			light->position.x,
			light->position.y,
			light->position.z
		};

		float direction[3] =
		{
			light->direction.x,
			light->direction.y,
			light->direction.z
		};

		float ambient[3] =
		{
			light->ambient.x,
			light->ambient.y,
			light->ambient.z
		};

		float diffuse[3] =
		{
			light->diffuse.x,
			light->diffuse.y,
			light->diffuse.z
		};

		float specular[3] =
		{
			light->specular.x,
			light->specular.y,
			light->specular.z
		};

		if (light->type != LightType::Directional)
		{
			if (ImGui::InputFloat3("Position", position))
			{
				light->position.x = position[0];
				light->position.y = position[1];
				light->position.z = position[2];
			}
		}

		if (ImGui::InputFloat3("Direction", direction))
		{
			light->direction.x = direction[0];
			light->direction.y = direction[1];
			light->direction.z = direction[2];
		}

		if (ImGui::ColorEdit3("Ambient", ambient))
		{
			light->ambient.x = ambient[0];
			light->ambient.y = ambient[1];
			light->ambient.z = ambient[2];
		}

		if (ImGui::ColorEdit3("Diffuse", diffuse))
		{
			light->diffuse.x = diffuse[0];
			light->diffuse.y = diffuse[1];
			light->diffuse.z = diffuse[2];
		}

		if (ImGui::ColorEdit3("Specular", specular))
		{
			light->specular.x = specular[0];
			light->specular.y = specular[1];
			light->specular.z = specular[2];
		}

		if (light->type != LightType::Directional)
		{
			ImGui::Separator();
			ImGui::Text("Attenuation");

			ImGui::InputFloat("Constant", &light->constantAttenuation);
			ImGui::InputFloat("Linear", &light->linearAttenuation);
			ImGui::InputFloat("Quadratic", &light->quadraticAttenuation);

			if (light->constantAttenuation < 0.0f) light->constantAttenuation = 0.0f;
			if (light->linearAttenuation < 0.0f) light->linearAttenuation = 0.0f;
			if (light->quadraticAttenuation < 0.0f) light->quadraticAttenuation = 0.0f;
		}

		if (light->type == LightType::Spotlight)
		{
			ImGui::Separator();
			ImGui::Text("Spotlight Settings");

			ImGui::InputFloat("Cutoff", &light->cutoff);
			ImGui::InputFloat("Exponent", &light->exponent);

			if (light->cutoff < 0.0f) light->cutoff = 0.0f;
			if (light->cutoff > 90.0f) light->cutoff = 90.0f;

			if (light->exponent < 0.0f) light->exponent = 0.0f;
			if (light->exponent > 128.0f) light->exponent = 128.0f;
		}

		ImGui::Separator();
		ImGui::Checkbox("Active", &light->active);
		ImGui::Checkbox("Visible", &light->visible);

		ImGui::End();
		return;
	}

	// =========================
	// GAMEOBJECT INSPECTOR
	// =========================
	if (scene->inspectedGameObject == nullptr)
	{
		ImGui::Text("No object selected.");
		ImGui::End();
		return;
	}

	GameObject* object = scene->inspectedGameObject;

	ImGui::Text("Object: %s", object->objectName.c_str());
	ImGui::Separator();

	float position[3] =
	{
		object->transform.position.x,
		object->transform.position.y,
		object->transform.position.z
	};

	float rotation[3] =
	{
		object->transform.rotation.x,
		object->transform.rotation.y,
		object->transform.rotation.z
	};

	float scale[3] =
	{
		object->transform.scale.x,
		object->transform.scale.y,
		object->transform.scale.z
	};

	if (ImGui::InputFloat3("Position", position))
	{
		object->transform.position.x = position[0];
		object->transform.position.y = position[1];
		object->transform.position.z = position[2];
	}

	if (ImGui::InputFloat3("Rotation", rotation))
	{
		object->transform.rotation.x = rotation[0];
		object->transform.rotation.y = rotation[1];
		object->transform.rotation.z = rotation[2];
	}

	if (ImGui::InputFloat3("Scale", scale))
	{
		if (scale[0] < 0.01f) scale[0] = 0.01f;
		if (scale[1] < 0.01f) scale[1] = 0.01f;
		if (scale[2] < 0.01f) scale[2] = 0.01f;

		object->transform.scale.x = scale[0];
		object->transform.scale.y = scale[1];
		object->transform.scale.z = scale[2];
	}

	ImGui::Separator();

	ImGui::Text("Type: %s",
		object->type == GameObjectType::Model ? "Model" : "Primitive");

	if (object->type == GameObjectType::Primitive)
	{
		switch (object->primitiveType)
		{
		case PrimitiveType::Cube:
			ImGui::Text("Primitive: Cube");
			break;
		case PrimitiveType::Sphere:
			ImGui::Text("Primitive: Sphere");
			break;
		default:
			ImGui::Text("Primitive: Unknown");
			break;
		}
	}

	ImGui::Checkbox("Visible", &object->visible);
	ImGui::Checkbox("Active", &object->active);

	ImGui::Separator();
	ImGui::Text("Material");

	float ambient[3] =
	{
		object->material.ambient.x,
		object->material.ambient.y,
		object->material.ambient.z
	};

	float diffuse[3] =
	{
		object->material.diffuse.x,
		object->material.diffuse.y,
		object->material.diffuse.z
	};

	float specular[3] =
	{
		object->material.specular.x,
		object->material.specular.y,
		object->material.specular.z
	};

	float shininess = object->material.shininess;

	if (ImGui::ColorEdit3("Mat Ambient", ambient))
	{
		object->material.ambient.x = ambient[0];
		object->material.ambient.y = ambient[1];
		object->material.ambient.z = ambient[2];
	}

	if (ImGui::ColorEdit3("Mat Diffuse", diffuse))
	{
		object->material.diffuse.x = diffuse[0];
		object->material.diffuse.y = diffuse[1];
		object->material.diffuse.z = diffuse[2];
	}

	if (ImGui::ColorEdit3("Mat Specular", specular))
	{
		object->material.specular.x = specular[0];
		object->material.specular.y = specular[1];
		object->material.specular.z = specular[2];
	}

	if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f))
	{
		object->material.shininess = shininess;
	}

	ImGui::Separator();
	ImGui::Text("Texture");

	ImGui::TextWrapped("Current: %s",
		object->modelTexturePath.empty() ? "None" : object->modelTexturePath.c_str());

	ImGui::TextWrapped("Browser Selection: %s", selectedTexturePath.empty() ? "None" : selectedTexturePath.c_str());

	if (ImGui::Button("Apply Selected Texture"))
	{
		if (!selectedTexturePath.empty())
		{
			object->modelTexturePath = selectedTexturePath;
			object->textureID = scene->textureManager.LoadTexture(selectedTexturePath);
		}
	}

	if (ImGui::Button("Remove Texture"))
	{
		object->modelTexturePath.clear();
		object->textureID = 0;
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