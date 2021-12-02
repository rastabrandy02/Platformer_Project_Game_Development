#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Player.h"
#include "Animation.h"
#include "Physics.h"
#include "Defs.h"
#include "Log.h"


#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	 

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();
	map = new Map();
	player = new Player();
	physics = new Physics();
	pathfinding = new PathFinding();
	walkingenemy = new WalkingEnemy();
	heartcontainer = new HeartContainer();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(physics);
	AddModule(scene);
	AddModule(map);
	AddModule(player);
	AddModule(pathfinding);
	AddModule(walkingenemy);
	AddModule(heartcontainer);
	// Render last to swap buffer
	AddModule(render);

	ptimer = new PerfTimer();
	frameDuration = new PerfTimer();
	maxFrameRate = 60;
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();

	configFile.reset();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	// L01: DONE 3: Load config from XML
	bool ret = LoadConfig();

	if(ret == true)
	{
		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		win->SetTitle(title.GetString());

		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}
	
	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	

	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	
	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = true;

	// L01: DONE 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result result = configFile.load_file("config.xml");

	// L01: DONE 3: Check result for loading errors
	if(result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = configFile.child("config");
		configApp = config.child("app");
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// Calculate the dt: differential time since last frame
	dt = frameDuration->ReadMs();
	frameDuration->Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: TODO 1: This is a good place to call Load / Save methods
	
	if (loadRequest)
	{
		LoadFromFile();
		loadRequest = false;
	}
	if (saveRequest)
	{
		SaveToFile();
		saveRequest = false;
	}

	float secondsSinceStartup = startupTime.ReadSec();

	if (lastSecFrameTime.Read() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	static char title[256];
	sprintf_s(title, 256, "Game Development Platformer Project - Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
		averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);
	dtSeconds = dt / 1000;
	// L08: DONE 2: Use SDL_Delay to make sure you get your capped framerate
	if (FPSCapTo30)maxFrameRate = 30;
	else maxFrameRate = 60;
	float delay = float(1000 / maxFrameRate) - frameDuration->ReadMs();
	//LOG("F: %f Delay:%f", frameDuration->ReadMs(), delay);

	// L08: DONE 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
	PerfTimer* delayt = new PerfTimer();
	delayt->Start();
	if (maxFrameRate > 0 && delay > 0) SDL_Delay(delay);
	
	LOG("Expected %f milliseconds and the real delay is % f", delay, delayt->ReadMs());

	app->win->SetTitle(title);
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// L02: TODO 1: Create methods to request load / save and methods for the real execution of load / save (to be implemented in TODO 5 and 7)
void App:: LoadGameRequest()
{
	loadRequest = true;
}
void App::SaveGameRequest() const
{
	saveRequest = true;
}
void App::LoadLevel1Request()
{

}

// L02: TODO 5: Implement the method LoadFromFile() to actually load an xml file
// then call all the modules to load themselves
bool App::LoadFromFile()
{
	bool ret = true;
	pugi::xml_parse_result result = gameStateFile.load_file("savegame.xml");
	//pugi::xml_node saveStateNode = gameStateFile.child("game_state");

	if (result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		//pugi::xml_node moduleNode = saveStateNode.child(item->data->name.GetString());
		ret = item->data->LoadState(gameStateFile.child("game_state").child(item->data->name.GetString()));
		//ret = item->data->LoadState(moduleNode);
		item = item->next;
	}
	
	loadRequest = false;
	return ret;
	

}
// L02: TODO 7: Implement the xml save method SaveToFile() for current state

bool App::SaveToFile() const
{
	bool ret = false;
	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("game_state");

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
	//pugi::xml_node moduleNode = saveStateNode.child(item->data->name.GetString());
		ret = item->data->SaveState(saveStateNode.append_child(item->data->name.GetString()));
		item = item->next;
	}
	ret = saveDoc->save_file("savegame.xml");
	saveRequest = false;
	return ret;

}

void App::ChangeScene(sceneEnum nextScene)
{
	currentScene = nextScene;
	
	/*CleanUp();
	Awake();*/
	
	Start();
	
}

void App::ChangeMaxFPS(uint32 newFPS)
{
	maxFrameRate = newFPS;
}
uint32 App::GetMaxFPS()
{
	return maxFrameRate;
}



