#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "Fireball.h"

bool Fireball::Awake(pugi::xml_node& config)
{
	return true;
}
Fireball::Fireball()
{
	
}
Fireball::Fireball(int x, int y, int lookingAt, int id)
{
	position.x = x;
	position.y = y;
	direction = lookingAt;
	this->id = id;
}
Fireball::~Fireball()
{

}
bool Fireball::Start()
{
	bool ret = true;
	
	if (app->currentScene == SCENE_LEVEL_1)
	{
		fireball = app->physics->CreateCircle(position.x, position.y, 20);
		fireBallTex = app->tex->Load("Assets/sprites/fireball.png");

		fireball->body->SetFixedRotation(true);
		fireball->body->SetType(b2_dynamicBody);
		fireball->body->GetFixtureList()->SetFriction(5.0f);
		fireball->body->SetGravityScale(0.0f);

		b2PolygonShape sensorShape;

		sensorShape.SetAsBox(PIXEL_TO_METERS(size / 2), PIXEL_TO_METERS(size / 2));


		b2FixtureDef sensorFix;
		sensorFix.shape = &sensorShape;
		sensorFix.isSensor = true;

		fireballSensor = fireball->body->CreateFixture(&sensorFix);
		fireballSensor->SetUserData((void*)DATA_FIREBALL);
		rec = { METERS_TO_PIXELS((int)fireball->body->GetPosition().x) - size,METERS_TO_PIXELS((int)fireball->body->GetPosition().y) + size, 60,60 };
		fireball->listener = this;

		if (direction == RIGHT)
		{
			fireball->body->SetLinearVelocity({ speed, 0.0f });
		}
		if (direction == LEFT)
		{
			fireball->body->SetLinearVelocity({ -speed, 0.0f });
		}
		app->player->fireballCount++;
		app->player->fireballZero = false;
	}
	return ret;
}
bool Fireball::PreUpdate()
{
	if(isAlive)deathTimer++;
	if (deathTimer > 75)
	{
		setToDestroy = true;
		deathTimer = 0;
	}
	
	
	

	return true;
}
bool Fireball::Update(float dt)
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive)
	{
		
		if (setToDestroy) Die();

	}
	
	return true;
}

bool Fireball::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive)
	{
		//app->render->DrawTexture(fireBallTex, METERS_TO_PIXELS(fireball->body->GetPosition().x) - 15, METERS_TO_PIXELS(fireball->body->GetPosition().y) - 15, &section);
		app->render->DrawCircle(METERS_TO_PIXELS(fireball->body->GetPosition().x), METERS_TO_PIXELS(fireball->body->GetPosition().y), 10, 255, 0, 0, 255);
		app->render->DrawTexture(fireBallTex, METERS_TO_PIXELS(fireball->body->GetPosition().x) - 35, METERS_TO_PIXELS(fireball->body->GetPosition().y) - 50);
		
		
	}
	return true;
}

void Fireball::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (isAlive)
	{
		LOG("Fireball Collision----------");
		setToDestroy = true;
	}
	
		
}
void Fireball::Die()
{
	
	app->physics->world->DestroyBody(fireball->body);
	setToDestroy = false;
	isAlive = false;

	
	

	app->player->fireballCount = app->player->fireballs.Count();
	if (app->player->fireballs.Count() == 0) app->player->fireballZero = true;
	
	
}
bool Fireball::CleanUp()
{
	return true;
}

bool Fireball::LoadState(pugi::xml_node&)
{
	return true;
}
bool Fireball::SaveState(pugi::xml_node&)
{
	return true;
}