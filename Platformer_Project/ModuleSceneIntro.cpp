#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	
	ground = { 0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 150 };
	
	//ground = { 0, 900, 500, 150 };

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::PreUpdate()
{
	
	return UPDATE_CONTINUE;
}
update_status ModuleSceneIntro::Update()
{
	
	SDL_SetRenderDrawColor(App->renderer->renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(App->renderer->renderer, &ground);
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{
	
	return UPDATE_CONTINUE;
}
