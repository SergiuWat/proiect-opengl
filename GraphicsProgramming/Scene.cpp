#include "Scene.h"
#include "imgui.h"
#include <cmath>
#include <limits>
#include <filesystem>
#include <string>
#include "ModelLoader.h"
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
}

void Scene::render() {

	// Clear Color and Depth Buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z, camera.getLookAt().x, camera.getLookAt().y, camera.getLookAt().z, camera.getUp().x, camera.getUp().y, camera.getUp().z);
	//gluLookAt(0, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// Render geometry/scene here -------------------------------------
	renderer.ApplyLights(lights, globalAmbient);
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
