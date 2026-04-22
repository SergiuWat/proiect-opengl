#include "Scene.h"
#include "imgui.h"
#include <cmath>
#include <limits>
#include <filesystem>
#include <string>
#include "ModelLoader.h"
#include "external/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

namespace fs = std::filesystem;
// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glutSetCursor(GLUT_CURSOR_NONE);

	// Initialise scene variables
	editorUI = new EditorUI(this);

	/*terrain.Generate(terrainWidth, terrainDepth, terrainCellSize, terrainHeightScale, terrainNoiseScale, terrainSeed);*/
	//terrainTextureID = textureManager.LoadTexture("gfx/grass.png");


	//globalAmbient = Vector3(0.35f, 0.35f, 0.35f);

	//Light* defaultLight = new Light();
	//defaultLight->name = "DefaultDirectional";
	//defaultLight->type = LightType::Directional;
	//defaultLight->active = true;
	//defaultLight->visible = true;
	//defaultLight->direction = Vector3(-1.0f, -1.0f, -0.5f);
	//defaultLight->ambient = Vector3(0.15f, 0.15f, 0.15f);
	//defaultLight->diffuse = Vector3(0.9f, 0.9f, 0.9f);
	//defaultLight->specular = Vector3(1.0f, 1.0f, 1.0f);

	//lights.push_back(defaultLight);



	LoadScene("scene.json");

	Light* fireLight = new Light();

	fireLight->name = "FireLight";
	fireLight->type = LightType::Point;

	fireLight->active = true;
	fireLight->visible = true;

	fireLight->position = Vector3(0.0f, 2.0f, 0.0f);

	fireLight->ambient = Vector3(0.2f, 0.05f, 0.0f);
	fireLight->diffuse = Vector3(1.0f, 0.3f, 0.1f);
	fireLight->specular = Vector3(1.0f, 0.5f, 0.2f);


	fireLight->constantAttenuation = 1.0f;
	fireLight->linearAttenuation = 0.09f;
	fireLight->quadraticAttenuation = 0.032f;

	lights.push_back(fireLight);
	skybox.loadTexture();
	
}

void Scene::handleInput(float dt)
{
	// Handle user input

	if (input->isKeyDown('o'))
	{
		input->setKeyUp('o');
		isDeveloperModeEnabled = !isDeveloperModeEnabled;
	}
	if (isDeveloperModeEnabled)
	{
		developerInputs(dt);
	}
	else
	{
		playerInputs(dt);
	}
	
}

void Scene::developerInputs(float dt)
{
	basicCameraInputs(dt);

	if (input->isMouseRDown())
	{
		input->setMouseRDown(false);

		if (selectedGameObject != nullptr)
		{
			gameObjects.push_back(selectedGameObject);
			selectedGameObject = nullptr;
			return;
		}

		if (selectedLight != nullptr)
		{
			lights.push_back(selectedLight);
			selectedLight = nullptr;
			return;
		}
	}
	if (input->isKeyDown('h'))
	{
		input->setKeyUp('h');
		editorMode = !editorMode;

	}
	if (input->isKeyDown('j'))
	{
		input->setKeyUp('j');
		snapEnabled = !snapEnabled;
	}
	if (input->isKeyDown(127)) // delete key
	{
		input->setKeyUp(127);
		if (selectedGameObject != nullptr)
		{
			delete selectedGameObject;
			selectedGameObject = nullptr;
			return;
		}

		if (selectedLight != nullptr)
		{
			delete selectedLight;
			selectedLight = nullptr;
			return;
		}
		if (inspectedGameObject != nullptr)
		{
			for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
			{
				if (*it == inspectedGameObject)
				{
					delete* it;
					gameObjects.erase(it);
					inspectedGameObject = nullptr;
					return;
				}
			}
		}
		if (inspectedLight != nullptr)
		{
			for (auto it = lights.begin(); it != lights.end(); ++it)
			{
				if (*it == inspectedLight)
				{
					delete* it;
					lights.erase(it);
					inspectedLight = nullptr;
					return;
				}
			}
		}
		if (!gameObjects.empty())
		{
			delete gameObjects.back();
			gameObjects.pop_back();
		}

		if (!lights.empty())
		{
			delete lights.back();
			lights.pop_back();
			return;
		}
	}
}

void Scene::playerInputs(float dt)
{
	basicCameraInputs(dt);
}

void Scene::basicCameraInputs(float dt)
{

	if (input->isKeyDown('w')) {
		camera.moveForward(dt);
	}
	if (input->isKeyDown('s')) {
		camera.moveBackward(dt);
	}
	if (input->isKeyDown('a')) {
		camera.moveLeft(dt);
	}
	if (input->isKeyDown('d')) {
		camera.moveRight(dt);
	}
	if (input->isKeyDown('q')) {
		camera.rotateLeft(dt);
	}
	if (input->isKeyDown('e')) {
		camera.rotateRight(dt);
	}
	if (input->isKeyDown('r')) {
		camera.rotateUp(dt);
	}
	if (input->isKeyDown('f')) {
		camera.rotateDown(dt);
	}
	if (!mouseLocked)
	{
		if (input->getMouseX() < width / 2) {
			camera.rotateLeft(dt);
		}
		if (input->getMouseX() > width / 2) {
			camera.rotateRight(dt);
		}
		if (input->getMouseY() < height / 2) {
			camera.rotateDown(dt);
		}
		if (input->getMouseY() > height / 2) {
			camera.rotateUp(dt);
		}
	}

	if (input->isKeyDown('l'))
	{
		input->setKeyUp('l');
		toggleMouseLocked();
	}
	
}

void Scene::toggleMouseLocked()
{
	mouseLocked = !mouseLocked;
	if (mouseLocked)
	{
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
	else
	{
		glutSetCursor(GLUT_CURSOR_NONE);
	}
}

void Scene::update(float dt)
{
	// update scene related variables.

	// Calculate FPS for output
	calculateFPS();
	if (!mouseLocked)
	{
		glutWarpPointer(width / 2, height / 2);
	}

	camera.Update();

	if (selectedGameObject != nullptr)
	{
		Vector3 camPos = camera.getPosition();
		Vector3 forward = camera.getForward();

		GameObject* objectInFront = nullptr;

		if (snapEnabled)
		{
			objectInFront = findObjectInFront();
		}

		if (objectInFront != nullptr)
		{
			Vector3 snapPos = calculateSnapPosition(objectInFront, selectedGameObject);
			selectedGameObject->transform.setPosition(snapPos);
		}
		else
		{
			float spawnDistance = 5.0f;
			Vector3 spawnPos(
				camPos.x + forward.x * spawnDistance,
				camPos.y + forward.y * spawnDistance,
				camPos.z + forward.z * spawnDistance
			);

			selectedGameObject->transform.setPosition(spawnPos);
		}
	}
	if (selectedLight != nullptr)
	{
		Vector3 camPos = camera.getPosition();
		Vector3 forward = camera.getForward();

		float spawnDistance = 5.0f;

		selectedLight->position = Vector3(
			camPos.x + forward.x * spawnDistance,
			camPos.y + forward.y * spawnDistance,
			camPos.z + forward.z * spawnDistance
		);

		selectedLight->direction = forward;
	}
	solarSystemRotation += solarSystemRotationSpeed * dt;
	if (solarSystemRotation > 360.0f)
	{
		solarSystemRotation -= 360.0f;
	}
}

void Scene::render() {

	// Clear Color and Depth Buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z, camera.getLookAt().x, camera.getLookAt().y, camera.getLookAt().z, camera.getUp().x, camera.getUp().y, camera.getUp().z);
	skybox.render(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	//gluLookAt(0, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// Render geometry/scene here -------------------------------------
	renderer.ApplyLights(lights, globalAmbient);

	if (showTerrain)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, terrainTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		terrain.Render();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	drawSolarSystem();
	renderer.ApplyMaterial(Vector3(0.2f, 0.2f, 0.2f), Vector3(0.8f, 0.8f, 0.8f), Vector3(0.0f, 0.0f, 0.0f), 1.0f);

	if (selectedGameObject != nullptr)
	{
		
		renderer.Render(selectedGameObject);
	}

	
	for (auto& gameObject : gameObjects)
	{
		renderer.Render(gameObject);
	}
	if (editorMode)
	{
		for (auto& light : lights)
		{
			renderer.RenderSphereAtLightLocation(light);
		}

		if (selectedLight != nullptr)
		{
			renderer.RenderSphereAtLightLocation(selectedLight);
		}
	}
	// End render geometry --------------------------------------

	// Render text, should be last object rendered.

	
	// Swap buffers, after all objects are rendered.
	//glutSwapBuffers();
}

void Scene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blending function

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void Scene::renderEditorUI()
{
	if (editorUI != nullptr)
	{
		editorUI->Render();
	}
}

void Scene::regenerateTerrain()
{
	terrain.Generate(
		terrainWidth,
		terrainDepth,
		terrainCellSize,
		terrainHeightScale,
		terrainNoiseScale,
		terrainSeed
	);
}


void Scene::createPrimitivePreview(PrimitiveType primitiveType)
{
	if (selectedGameObject != nullptr)
	{
		delete selectedGameObject;
		selectedGameObject = nullptr;
	}
	// de completat si id-ul
	selectedGameObject = new GameObject();
	selectedGameObject->type = GameObjectType::Primitive;
	selectedGameObject->primitiveType = primitiveType;
	selectedGameObject->objectName = primitiveTypeToString(primitiveType);
	selectedGameObject->active = true;
	selectedGameObject->visible = true;

}

const char* Scene::primitiveTypeToString(PrimitiveType primitiveType)
{
	switch (primitiveType)
	{
	case PrimitiveType::Cube:
		return "Cube_";
	case PrimitiveType::Sphere:
		return "Sphere_";
	default:
		return "Unknown";
	}
}


GameObject* Scene::findObjectInFront()
{
	GameObject* closestObject = nullptr;
	float closestDistance = std::numeric_limits<float>::max();

	Vector3 camPos = camera.getPosition();
	Vector3 forward = camera.getForward();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject == nullptr)
			continue;

		Vector3 objPos = gameObject->transform.getPosition();

		Vector3 toObject(objPos.x - camPos.x, objPos.y - camPos.y, objPos.z - camPos.z);

		float distance = sqrt(toObject.x * toObject.x +toObject.y * toObject.y +toObject.z * toObject.z);

		if (distance > snapSearchDistance)
			continue;

		if (distance > 0.0001f)
		{
			toObject.x /= distance;
			toObject.y /= distance;
			toObject.z /= distance;
		}

		float dot = forward.x * toObject.x + forward.y * toObject.y + forward.z * toObject.z;

		if (dot > 0.95f)
		{
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestObject = gameObject;
			}
		}
	}

	return closestObject;
}
GameObject* Scene::objectLoad(std::string modelPath, std::string texturePath)
{
	GameObject* newObject = new GameObject();

	newObject->type = GameObjectType::Model;
	newObject->modelPath = modelPath;
	newObject->modelTexturePath = texturePath;

	newObject->model = new ModelLoader();

	if (!newObject->model->LoadModel(modelPath.c_str()))
	{
		delete newObject->model;
		delete newObject;
		return nullptr;
	}

	newObject->textureID = textureManager.LoadTexture(texturePath);

	newObject->transform.position = Vector3(0.0f, 0.0f, 0.0f);
	newObject->transform.rotation = Vector3(0.0f, 0.0f, 0.0f);
	newObject->transform.scale = Vector3(1.0f, 1.0f, 1.0f);
	newObject->objectName = fs::path(modelPath).stem().string() + "_" + std::to_string(gameObjects.size() - 1);

	return newObject;
}
Vector3 Scene::calculateSnapPosition(GameObject* target, GameObject* objectToPlace)
{
	Vector3 targetPos = target->transform.getPosition();
	Vector3 targetScale = target->transform.getScale();
	Vector3 placedScale = objectToPlace->transform.getScale();

	Vector3 forward = camera.getForward();
	Vector3 newPos = targetPos;

	float absX = std::abs(forward.x);
	float absY = std::abs(forward.y);
	float absZ = std::abs(forward.z);

	if (absX >= absY && absX >= absZ)
	{
		float offset = (targetScale.x * 0.5f) + (placedScale.x * 0.5f);

		if (forward.x >= 0.0f)
			newPos.x += offset;
		else
			newPos.x -= offset;
	}
	else if (absY >= absX && absY >= absZ)
	{
		float offset = (targetScale.y * 0.5f) + (placedScale.y * 0.5f);

		if (forward.y >= 0.0f)
			newPos.y += offset;
		else
			newPos.y -= offset;
	}
	else
	{
		float offset = (targetScale.z * 0.5f) + (placedScale.z * 0.5f);

		if (forward.z >= 0.0f)
			newPos.z += offset;
		else
			newPos.z -= offset;
	}

	return newPos;
}
void Scene::createLightPreview(LightType lightType)
{
	if (selectedLight != nullptr)
	{
		delete selectedLight;
		selectedLight = nullptr;
	}

	selectedLight = new Light();
	selectedLight->type = lightType;
	selectedLight->name = lightTypeToString(lightType);
	selectedLight->active = true;
	selectedLight->visible = true;

	if (lightType == LightType::Directional)
	{
		selectedLight->direction = camera.getForward();
	}
	else
	{
		selectedLight->position = camera.getPosition();
		selectedLight->direction = camera.getForward();
	}
}
const char* Scene::lightTypeToString(LightType lightType)
{
	switch (lightType)
	{
	case LightType::Directional:
		return "DirectionalLight";
	case LightType::Spotlight:
		return "SpotLight";
	case LightType::Point:
		return "PointLight";
	default:
		return "UnknownLight";
	}
}
bool Scene::SaveScene(const std::string& filePath)
{
	json sceneJson;

	sceneJson["terrain"]["showTerrain"] = showTerrain;
	sceneJson["terrain"]["terrainSeed"] = terrainSeed;
	sceneJson["terrain"]["terrainWidth"] = terrainWidth;
	sceneJson["terrain"]["terrainDepth"] = terrainDepth;
	sceneJson["terrain"]["terrainCellSize"] = terrainCellSize;
	sceneJson["terrain"]["terrainHeightScale"] = terrainHeightScale;
	sceneJson["terrain"]["terrainNoiseScale"] = terrainNoiseScale;
	sceneJson["terrain"]["terrainTexturePath"] = "gfx/grass.png";

	sceneJson["globalAmbient"] = {
		globalAmbient.x,
		globalAmbient.y,
		globalAmbient.z
	};

	sceneJson["objects"] = json::array();

	for (GameObject* object : gameObjects)
	{
		if (object == nullptr)
			continue;

		json objectJson;

		objectJson["id"] = object->id;
		objectJson["objectName"] = object->objectName;
		objectJson["active"] = object->active;
		objectJson["visible"] = object->visible;
		objectJson["selected"] = object->selected;

		objectJson["type"] = static_cast<int>(object->type);
		objectJson["primitiveType"] = static_cast<int>(object->primitiveType);

		objectJson["modelPath"] = object->modelPath;
		objectJson["modelTexturePath"] = object->modelTexturePath;

		objectJson["transform"]["position"] = {
			object->transform.position.x,
			object->transform.position.y,
			object->transform.position.z
		};

		objectJson["transform"]["rotation"] = {
			object->transform.rotation.x,
			object->transform.rotation.y,
			object->transform.rotation.z
		};

		objectJson["transform"]["scale"] = {
			object->transform.scale.x,
			object->transform.scale.y,
			object->transform.scale.z
		};

		objectJson["material"]["ambient"] = {
			object->material.ambient.x,
			object->material.ambient.y,
			object->material.ambient.z
		};

		objectJson["material"]["diffuse"] = {
			object->material.diffuse.x,
			object->material.diffuse.y,
			object->material.diffuse.z
		};

		objectJson["material"]["specular"] = {
			object->material.specular.x,
			object->material.specular.y,
			object->material.specular.z
		};

		objectJson["material"]["shininess"] = object->material.shininess;

		sceneJson["objects"].push_back(objectJson);
	}

	sceneJson["lights"] = json::array();

	for (Light* light : lights)
	{
		if (light == nullptr)
			continue;

		json lightJson;

		lightJson["name"] = light->name;
		lightJson["type"] = static_cast<int>(light->type);
		lightJson["active"] = light->active;
		lightJson["visible"] = light->visible;

		lightJson["position"] = {
			light->position.x,
			light->position.y,
			light->position.z
		};

		lightJson["direction"] = {
			light->direction.x,
			light->direction.y,
			light->direction.z
		};

		lightJson["ambient"] = {
			light->ambient.x,
			light->ambient.y,
			light->ambient.z
		};

		lightJson["diffuse"] = {
			light->diffuse.x,
			light->diffuse.y,
			light->diffuse.z
		};

		lightJson["specular"] = {
			light->specular.x,
			light->specular.y,
			light->specular.z
		};

		lightJson["constantAttenuation"] = light->constantAttenuation;
		lightJson["linearAttenuation"] = light->linearAttenuation;
		lightJson["quadraticAttenuation"] = light->quadraticAttenuation;
		lightJson["cutoff"] = light->cutoff;
		lightJson["exponent"] = light->exponent;

		sceneJson["lights"].push_back(lightJson);
	}

	std::ofstream outFile(filePath);
	if (!outFile.is_open())
		return false;

	outFile << sceneJson.dump(4);
	outFile.close();

	return true;
}
bool Scene::LoadScene(const std::string& filePath)
{
	std::ifstream inFile(filePath);
	if (!inFile.is_open())
		return false;

	json sceneJson;
	inFile >> sceneJson;
	inFile.close();

	ClearScene();

	if (sceneJson.contains("terrain"))
	{
		terrainSeed = sceneJson["terrain"].value("terrainSeed", 6767);
		terrainWidth = sceneJson["terrain"].value("terrainWidth", 200);
		terrainDepth = sceneJson["terrain"].value("terrainDepth", 200);
		terrainCellSize = sceneJson["terrain"].value("terrainCellSize", 1.0f);
		terrainHeightScale = sceneJson["terrain"].value("terrainHeightScale", 2.0f);
		terrainNoiseScale = sceneJson["terrain"].value("terrainNoiseScale", 0.05f);
		showTerrain = sceneJson["terrain"].value("showTerrain", true);

		terrain.Generate(
			terrainWidth,
			terrainDepth,
			terrainCellSize,
			terrainHeightScale,
			terrainNoiseScale,
			terrainSeed
		);

		std::string terrainTexturePath = sceneJson["terrain"].value("terrainTexturePath", std::string("gfx/grass.png"));
		terrainTextureID = textureManager.LoadTexture(terrainTexturePath);
	}

	if (sceneJson.contains("globalAmbient") && sceneJson["globalAmbient"].is_array() && sceneJson["globalAmbient"].size() == 3)
	{
		globalAmbient.x = sceneJson["globalAmbient"][0];
		globalAmbient.y = sceneJson["globalAmbient"][1];
		globalAmbient.z = sceneJson["globalAmbient"][2];
	}

	if (sceneJson.contains("objects"))
	{
		for (const auto& objectJson : sceneJson["objects"])
		{
			GameObject* object = new GameObject();

			object->id = objectJson.value("id", 0);
			object->objectName = objectJson.value("objectName", std::string("GameObject"));
			object->active = objectJson.value("active", true);
			object->visible = objectJson.value("visible", true);
			object->selected = objectJson.value("selected", false);

			object->type = static_cast<GameObjectType>(objectJson.value("type", 0));
			object->primitiveType = static_cast<PrimitiveType>(objectJson.value("primitiveType", 0));

			object->modelPath = objectJson.value("modelPath", std::string(""));
			object->modelTexturePath = objectJson.value("modelTexturePath", std::string(""));

			if (objectJson.contains("transform"))
			{
				const auto& transformJson = objectJson["transform"];

				if (transformJson.contains("position"))
				{
					object->transform.position.x = transformJson["position"][0];
					object->transform.position.y = transformJson["position"][1];
					object->transform.position.z = transformJson["position"][2];
				}

				if (transformJson.contains("rotation"))
				{
					object->transform.rotation.x = transformJson["rotation"][0];
					object->transform.rotation.y = transformJson["rotation"][1];
					object->transform.rotation.z = transformJson["rotation"][2];
				}

				if (transformJson.contains("scale"))
				{
					object->transform.scale.x = transformJson["scale"][0];
					object->transform.scale.y = transformJson["scale"][1];
					object->transform.scale.z = transformJson["scale"][2];
				}
			}

			if (objectJson.contains("material"))
			{
				const auto& materialJson = objectJson["material"];

				if (materialJson.contains("ambient"))
				{
					object->material.ambient.x = materialJson["ambient"][0];
					object->material.ambient.y = materialJson["ambient"][1];
					object->material.ambient.z = materialJson["ambient"][2];
				}

				if (materialJson.contains("diffuse"))
				{
					object->material.diffuse.x = materialJson["diffuse"][0];
					object->material.diffuse.y = materialJson["diffuse"][1];
					object->material.diffuse.z = materialJson["diffuse"][2];
				}

				if (materialJson.contains("specular"))
				{
					object->material.specular.x = materialJson["specular"][0];
					object->material.specular.y = materialJson["specular"][1];
					object->material.specular.z = materialJson["specular"][2];
				}

				object->material.shininess = materialJson.value("shininess", 32.0f);
			}

			if (object->type == GameObjectType::Model && !object->modelPath.empty())
			{
				object->model = new ModelLoader();
				object->model->LoadModel(object->modelPath.c_str());
			}

			if (!object->modelTexturePath.empty())
			{
				object->textureID = textureManager.LoadTexture(object->modelTexturePath);
			}

			gameObjects.push_back(object);
		}
	}

	if (sceneJson.contains("lights"))
	{
		for (const auto& lightJson : sceneJson["lights"])
		{
			Light* light = new Light();

			light->name = lightJson.value("name", std::string("Light"));
			light->type = static_cast<LightType>(lightJson.value("type", 0));
			light->active = lightJson.value("active", true);
			light->visible = lightJson.value("visible", true);

			if (lightJson.contains("position"))
			{
				light->position.x = lightJson["position"][0];
				light->position.y = lightJson["position"][1];
				light->position.z = lightJson["position"][2];
			}

			if (lightJson.contains("direction"))
			{
				light->direction.x = lightJson["direction"][0];
				light->direction.y = lightJson["direction"][1];
				light->direction.z = lightJson["direction"][2];
			}

			if (lightJson.contains("ambient"))
			{
				light->ambient.x = lightJson["ambient"][0];
				light->ambient.y = lightJson["ambient"][1];
				light->ambient.z = lightJson["ambient"][2];
			}

			if (lightJson.contains("diffuse"))
			{
				light->diffuse.x = lightJson["diffuse"][0];
				light->diffuse.y = lightJson["diffuse"][1];
				light->diffuse.z = lightJson["diffuse"][2];
			}

			if (lightJson.contains("specular"))
			{
				light->specular.x = lightJson["specular"][0];
				light->specular.y = lightJson["specular"][1];
				light->specular.z = lightJson["specular"][2];
			}

			light->constantAttenuation = lightJson.value("constantAttenuation", 1.0f);
			light->linearAttenuation = lightJson.value("linearAttenuation", 0.09f);
			light->quadraticAttenuation = lightJson.value("quadraticAttenuation", 0.032f);
			light->cutoff = lightJson.value("cutoff", 20.0f);
			light->exponent = lightJson.value("exponent", 16.0f);

			lights.push_back(light);
		}
	}

	return true;
}

void Scene::ClearScene()
{
	for (GameObject* object : gameObjects)
	{
		delete object;
	}
	gameObjects.clear();

	for (Light* light : lights)
	{
		delete light;
	}
	lights.clear();

	if (selectedGameObject != nullptr)
	{
		delete selectedGameObject;
		selectedGameObject = nullptr;
	}

	if (selectedLight != nullptr)
	{
		delete selectedLight;
		selectedLight = nullptr;
	}

	inspectedGameObject = nullptr;
	inspectedLight = nullptr;
}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

void Scene::drawSolarSystem()
{
	glPushMatrix();

	glTranslatef(solarSystemPosition.x, solarSystemPosition.y, solarSystemPosition.z);

	drawSun();

	glPushMatrix();
	drawEarth();
	glPopMatrix();

	glPushMatrix();
	drawEarth2();
	glPopMatrix();

	glPopMatrix();
}

void Scene::drawSun()
{
	glPushMatrix();

	renderer.ApplyMaterial(Vector3(1.0f, 0.8f, 0.2f), Vector3(1.0f, 0.9f, 0.3f), Vector3(1.0f, 1.0f, 0.5f), 10.0f);
	GLfloat emission[] = { 1.0f, 0.8f, 0.2f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glScalef(1.5f, 1.5f, 1.5f);
	proceduralShapes.renderSphere();

	glPopMatrix();
}

void Scene::drawEarth()
{
	glRotatef(solarSystemRotation, 0, 0, 1);
	glTranslatef(10.0f, 0.0f, 0.0f);

	glPushMatrix();

	renderer.ApplyMaterial(Vector3(0.0f, 0.0f, 0.2f), Vector3(0.0f, 0.0f, 0.8f), Vector3(0.3f, 0.3f, 1.0f), 32.0f);
	glScalef(1.2f, 1.2f, 1.2f);
	proceduralShapes.renderSphere();

	glPopMatrix();
}

void Scene::drawEarth2()
{
	glRotatef(solarSystemRotation, 0, 0, 1);
	glTranslatef(10.0f, 0.0f, 0.0f);

	glPushMatrix();

	renderer.ApplyMaterial(Vector3(0.2f, 0.0f, 0.0f), Vector3(0.8f, 0.1f, 0.1f), Vector3(1.0f, 0.3f, 0.3f), 16.0f);
	glScalef(1.4f, 1.4f, 1.4f);
	proceduralShapes.renderSphere();

	glPopMatrix();

	glPushMatrix();
	drawEarth2FirstMoon();
	glPopMatrix();

	glPushMatrix();
	drawEarth2SecondMoon();
	glPopMatrix();
}

void Scene::drawEarth2FirstMoon()
{
	glRotatef(solarSystemRotation * 2.0f, 0, 0, 1);
	glTranslatef(7.0f, 0.0f, 0.0f);

	glPushMatrix();

	renderer.ApplyMaterial(Vector3(0.2f, 0.2f, 0.2f), Vector3(0.6f, 0.6f, 0.6f), Vector3(0.3f, 0.3f, 0.3f), 8.0f);
	glScalef(1.1f, 1.1f, 1.1f);
	proceduralShapes.renderSphere();

	glPopMatrix();

	glPushMatrix();
	drawEarth2MoonWithAMoon();
	glPopMatrix();
}

void Scene::drawEarth2SecondMoon()
{
	glRotatef(solarSystemRotation * 1.5f, 0, 1, 0);
	glTranslatef(7.2f, 0.0f, 0.0f);

	glPushMatrix();

	renderer.ApplyMaterial(Vector3(0.2f, 0.2f, 0.2f), Vector3(0.6f, 0.6f, 0.6f), Vector3(0.3f, 0.3f, 0.3f), 8.0f);

	glScalef(1.12f, 1.12f, 1.12f);
	proceduralShapes.renderSphere();

	glPopMatrix();
}

void Scene::drawEarth2MoonWithAMoon()
{
	glRotatef(solarSystemRotation * 3.0f, 0, 1, 0);
	glTranslatef(6.4f, 0.0f, 0.0f);

	glPushMatrix();

	renderer.ApplyMaterial(Vector3(0.2f, 0.2f, 0.2f), Vector3(0.6f, 0.6f, 0.6f), Vector3(0.3f, 0.3f, 0.3f), 8.0f);
	glScalef(0.15f, 0.15f, 0.15f);
	proceduralShapes.renderSphere();

	glPopMatrix();
}
