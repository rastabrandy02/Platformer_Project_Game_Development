#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "FlyingEnemy.h"

FlyingEnemy::FlyingEnemy() : Module()
{

	name.Create("Flying_Enemy");
}

FlyingEnemy::~FlyingEnemy()
{

}

// Load assets
bool FlyingEnemy::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemy");

	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();

	return true;
}

bool FlyingEnemy::Start()
{
	bool ret = true;

	flyingEnemy = app->tex->Load("Assets/sprites/flyingenemy_sprite.png");

	if (app->currentScene == SCENE_LEVEL_1)
	{
		enemy = app->physics->CreateCircle(400, 50, 25);

		enemy->body->SetFixedRotation(true);
		enemy->body->GetFixtureList()->SetFriction(5.0f);

		b2PolygonShape sensorShape;
		sensorShape.SetAsBox(PIXEL_TO_METERS(30), PIXEL_TO_METERS(30));


		b2FixtureDef sensorFix;
		sensorFix.shape = &sensorShape;
		sensorFix.isSensor = true;

		enemySensor = enemy->body->CreateFixture(&sensorFix);
		enemySensor->SetUserData((void*)DATA_ENEMY);
		enemyRec = { METERS_TO_PIXELS((int)enemy->body->GetPosition().x) - 60,METERS_TO_PIXELS((int)enemy->body->GetPosition().y) + 60, 60, 60 };
		enemy->listener = app->flyingenemy;

		pathfinding = new PathFinding();
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &data)) pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

		navigationPath = app->tex->Load("Assets/maps/navigation_path.png");
	}
	return ret;	
}

bool FlyingEnemy::PreUpdate()
{
	return true;
}

bool FlyingEnemy::Update(float dt)
{
	if (app->currentScene == SCENE_LEVEL_1)
	{
		iPoint origin = { (int)METERS_TO_PIXELS((int)enemy->body->GetPosition().x), (int)METERS_TO_PIXELS((int)enemy->body->GetPosition().y) };
		iPoint destination = { (int)METERS_TO_PIXELS((int)app->player->player->body->GetPosition().x), (int)METERS_TO_PIXELS((int)app->player->player->body->GetPosition().y) };

		origin = app->map->WorldToMap(origin.x, origin.y);
		destination = app->map->WorldToMap(destination.x, destination.y);

		pathfinding->CreatePath(origin, destination);
		Walk();

	}
	return true;
}

bool FlyingEnemy::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1)
	{
		SDL_Rect section = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(flyingEnemy, 255, 0, 0, 255);

		//app->render->DrawRectangle(enemyRec, 255, 0, 0, 255);

		for (uint i = 0; i < currentPath->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(currentPath->At(i)->x, currentPath->At(i)->y);
			app->render->DrawTexture(navigationPath, pos.x, pos.y);
		}
	}
	return true;
}

void FlyingEnemy::Walk()
{
	currentPath = pathfinding->GetLastPath();

	iPoint nextStep = { currentPath->At(1)->x, currentPath->At(1)->y };
	nextStep = app->map->MapToWorld(nextStep.x, nextStep.y);
	nextStep.x = PIXEL_TO_METERS(nextStep.x);
	nextStep.y = PIXEL_TO_METERS(nextStep.y);


	if (enemy->body->GetPosition().x >= nextStep.x)
	{
		enemy->body->SetLinearVelocity({ -speed.x ,enemy->body->GetLinearVelocity().y });
	}
	if (enemy->body->GetPosition().x < nextStep.x)
	{
		enemy->body->SetLinearVelocity({ speed.x,enemy->body->GetLinearVelocity().y });
	}


	enemyRec.x = METERS_TO_PIXELS(enemy->body->GetPosition().x) - 30;
	enemyRec.y = METERS_TO_PIXELS(enemy->body->GetPosition().y) - 30;
}

void FlyingEnemy::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int otherData = (int)bodyB->body->GetUserData();
	if (otherData == DATA_PLAYER)
	{
		app->player->TakeDamage(1);
	}
}

bool FlyingEnemy::LoadState(pugi::xml_node&)
{
	return true;
}

bool FlyingEnemy::SaveState(pugi::xml_node&)
{
	return true;
}

bool FlyingEnemy::CleanUp()
{
	return true;
}