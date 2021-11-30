#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "WalkingEnemy.h"

/*WalkingEnemy::WalkingEnemy() : Module()
{
	//animations

	name.Create("Walking Enemy");
}

WalkingEnemy::~WalkingEnemy()
{}

// Load assets
bool WalkingEnemy::Awake(pugi::xml_node& config)
{
	LOG("Loading player");

	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();

	return true;
}
bool Player::Start()
{
	bool ret = true;

	//wizard = app->tex->Load("Assets/sprites/wizard_spritesheet.png");
	wizard = app->tex->Load("Assets/sprites/walkingenemy_spritesheet_extended.png");
	if (app->currentScene == SCENE_LEVEL_1)
	{
		enemy = app->physics->CreateCircle(position.x, position.y, 25);

		enemy->body->SetFixedRotation(true);
		enemy->body->GetFixtureList()->SetFriction(5.0f);

		b2PolygonShape sensorShape;
		sensorShape.SetAsBox(PIXEL_TO_METERS(26), PIXEL_TO_METERS(26));


		b2FixtureDef sensorFixture;
		sensorFixture.shape = &sensorShape;
		sensorFixture.isSensor = true;

		playerSensor = enemy->body->CreateFixture(&sensorFixture);
		playerSensor->SetUserData((void*)DATA_PLAYER);
	}




	return ret;
}
*/