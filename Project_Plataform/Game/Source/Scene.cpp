#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	if (app->currentScene == SCENE_TITLE)
	{
		
		
		titlleBackground = app->tex->Load("Assets/maps/TitleScreen.png");

	}
	
	if (app->currentScene == SCENE_LEVEL_1)
	{
		app->entityManager->CreateEntity(PLAYER, NULL, NULL);
		app->entityManager->CreateEntity(WALKING_ENEMY, 400, 200);
		app->entityManager->CreateEntity(FLYING_ENEMY, 600, 100);
		app->entityManager->CreateEntity(HEART, 800, 200);
		app->entityManager->CreateEntity(STAR, 1090, 200);
		

		if (app->map->Load("WizardMap.tmx") == true)
		{
		}
		app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

		

	}
	if (app->currentScene == SCENE_DEATH)
	{
		deathBackground = app->tex->Load("Assets/maps/DeathScreen.png");
	}
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1)
	{
		
	}
	
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L02: TODO 3: Request App to Load / Save when pressing the keys L (load) / S (save)

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1 * dt;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1 * dt;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1 * dt;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1 * dt;
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{

	}
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		app->FPSCapTo30 = !app->FPSCapTo30;
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->currentScene == SCENE_TITLE)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE))
		{
			app->ChangeScene(SCENE_LEVEL_1);
		}
	}
	if (app->currentScene == SCENE_DEATH)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE))
		{
			return false;
		}
	}
	
	
	if (app->currentScene == SCENE_TITLE)
	{
		app->render->DrawTexture(titlleBackground, 0, 0);
	}
	if (app->currentScene == SCENE_LEVEL_1)
	{
		app->map->Draw();
		
		
	}
	if (app->currentScene == SCENE_DEATH)
	{
		app->render->DrawTexture(deathBackground, 0, 0);
	}
	
	
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	
	if (app->currentScene == SCENE_LEVEL_1)
	{
		
	}
	
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	
	LOG("Freeing scene");

	return true;
}
bool Scene::SaveState(pugi::xml_node& node)
{
	for (ListItem<Module*>* item = enemies.start; item; item = item->next)
	{
		SString name = item->data->name.GetString();
		item->data->SaveState(node.parent().append_child(name.GetString()));
	}
	

	return true;
}
bool Scene::LoadState(pugi::xml_node& node)
{
	for (ListItem<Module*>* item = enemies.start; item; item = item->next)
	{
		SString name = item->data->name.GetString();
		item->data->LoadState(node.parent().child(name.GetString()));
	}
	return true;
}
