// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Input.h"
#include <stdio.h>
// Further includes should go here:
#include "SOIL.h"
#include <vector>
#include "Camera.h"
#include "GameObject.h"
#include "GameObjectTypes.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "EditorUI.h"
#include "Light.h"
#include "Terrain.h"



class Scene{

public:

	friend class EditorUI;

	Scene(Input *in);
	// Main render function
	void render();
	void renderEditorUI();

	std::string selectedModelPath;
	std::string selectedTexturePath;

	// Handle input function that receives delta time from parent.
	void handleInput(float dt);

	void developerInputs(float dt);
	void playerInputs(float dt);
	void basicCameraInputs(float dt);
	void toggleMouseLocked();
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

	Camera camera;
	bool mouseLocked = false;
	bool isDeveloperModeEnabled = true;
	bool editorMode = true;
	bool snapEnabled = true;

protected:
	// configure opengl render pipeline
	void initialiseOpenGL();
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();

	void calculateFPS();

	// draw primitive functions
	

	// For access to user input.
	Input* input;
		
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;


	Renderer renderer;
	TextureManager textureManager;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];


	//Edit functions
	EditorUI* editorUI;


	void createPrimitivePreview(PrimitiveType primitiveType);
	const char* primitiveTypeToString(PrimitiveType primitiveType);
	// Gameobjects variables

	std::vector<GameObject*> gameObjects;
	GameObject* selectedGameObject = nullptr;
	GameObject* inspectedGameObject = nullptr;


	GameObject* findObjectInFront();
	GameObject* objectLoad(std::string modelPath, std::string texturePath);
	Vector3 calculateSnapPosition(GameObject* target, GameObject* objectToPlace);

	Terrain terrain;
	int terrainSeed = 6767;
	unsigned int terrainTextureID = 0;
	bool showTerrain = true;

	int terrainWidth = 200;
	int terrainDepth = 200;
	float terrainCellSize = 1.0f;
	float terrainHeightScale = 2.0f;
	float terrainNoiseScale = 0.05f;
	void regenerateTerrain();

	// LIGHTS
	std::vector<Light*> lights;
	Vector3 globalAmbient = Vector3(0.2f, 0.2f, 0.2f);
	Light* selectedLight = nullptr;   
	Light* inspectedLight = nullptr;  
	void createLightPreview(LightType lightType);
	const char* lightTypeToString(LightType lightType);
	// LIGHTS
	float snapSearchDistance = 20.0f;

	bool SaveScene(const std::string& filePath);
	bool LoadScene(const std::string& filePath);
	void ClearScene();
};

#endif