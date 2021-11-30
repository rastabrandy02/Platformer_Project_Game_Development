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
		
		//titlleBackground = app->tex->Load("Assets/maps/SceneTitle.png");
		titlleBackground = app->tex->Load("Assets/maps/TitleScreen.png");

	}
	
	if (app->currentScene == SCENE_LEVEL_1)
	{
		navigationPath = app->tex->Load("Assets/maps/navigation_path.png");
		if (app->map->Load("WizardMap.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);

			enemy = app->physics->CreateCircle(400, 50, 25);

			enemy->body->SetFixedRotation(true);
			enemy->body->GetFixtureList()->SetFriction(5.0f);

			b2PolygonShape sensorShape;
			sensorShape.SetAsBox(PIXEL_TO_METERS(30), PIXEL_TO_METERS(30));


			b2FixtureDef sensorFixture;
			sensorFixture.shape = &sensorShape;
			sensorFixture.isSensor = true;

			enemySensor = enemy->body->CreateFixture(&sensorFixture);
			enemySensor->SetUserData((void*)DATA_ENEMY);
			enemyRec = {METERS_TO_PIXELS( (int)enemy->body->GetPosition().x) - 60,METERS_TO_PIXELS((int)enemy->body->GetPosition().y) + 60, 60,60 };
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
	//app->render->DrawTexture(img, 380, 100);
	//app->render->DrawTexture(app->player->wizard, position.x, position.y);
	
	if (app->currentScene == SCENE_TITLE)
	{
		app->render->DrawTexture(titlleBackground, 0, 0);
	}
	if (app->currentScene == SCENE_LEVEL_1)
	{
		app->map->Draw();
		/*iPoint origin = {(int) METERS_TO_PIXELS(enemy->body->GetPosition().x), (int)METERS_TO_PIXELS(enemy->body->GetPosition().y) };
		iPoint destination = { (int)METERS_TO_PIXELS(app->player->player->body->GetPosition().x), (int)METERS_TO_PIXELS(app->player->player->body->GetPosition().y) };*/
		iPoint origin = { (int)enemy->body->GetPosition().x, (int)enemy->body->GetPosition().y };
		iPoint destination = { (int)app->player->player->body->GetPosition().x, (int)app->player->player->body->GetPosition().y }; 
		app->pathfinding->CreatePath(origin, destination);
		

		enemyRec.x = METERS_TO_PIXELS (enemy->body->GetPosition().x) -30;
		enemyRec.y = METERS_TO_PIXELS (enemy->body->GetPosition().y) -30;
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
	
	SDL_Rect r = { 0, 700, 1500, 20 };
	app->render->DrawRectangle(r, 0, 255, 0, 255);
	if (app->currentScene == SCENE_LEVEL_1)
	{
		app->render->DrawRectangle(enemyRec, 255, 0, 0, 255);
		
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(navigationPath, pos.x, pos.y);
		}
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
