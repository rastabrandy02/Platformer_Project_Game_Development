#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Animation.h"

#include "Defs.h"
#include "Log.h"



#include "PugiXml/src/pugixml.hpp"
#include "SDL/include/SDL.h"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Player;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // L02: TODO 1: Create methods to request Load / Save the game state
	void LoadGameRequest();
	void SaveGameRequest() const;
	bool LoadFromFile();
	bool SaveToFile() const;

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	Player* player;

#define SCREEN_HEIGHT 758
#define SCREEN_WIDTH 1024

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	// L01: DONE 2: Create new variables from pugui namespace:
	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_document gameStateFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	uint frames;
	float dt;

	// L02: TODO 1: Create methods to request Load and Save and control variables to make sure that executes the real Load and Save at the end of the frame
	mutable bool saveRequest;
	bool loadRequest;
};

extern App* app;

#endif	// __APP_H__