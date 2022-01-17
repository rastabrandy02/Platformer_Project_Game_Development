#include "App.h"
#include "Star.h"

Star::Star(int x, int y)
{
	position.x = x;
	position.y = y;
	name.Create("star");
}
Star::~Star()
{

}

bool Star::Awake(pugi::xml_node& config)
{
	LOG("Loading Hearts");

	

	return true;
}

bool Star::Start()
{
	bool ret = true;



	if (app->currentScene == SCENE_LEVEL_1)
	{
		starPb = app->physics->CreateCircle(position.x, position.y, 25);

		starPb->body->SetFixedRotation(true);
		starPb->body->GetFixtureList()->SetFriction(5.0f);

		b2PolygonShape sensorShape;
		sensorShape.SetAsBox(PIXEL_TO_METERS(30), PIXEL_TO_METERS(30));


		b2FixtureDef sensorFix;
		sensorFix.shape = &sensorShape;
		sensorFix.isSensor = true;

		starSensor = starPb->body->CreateFixture(&sensorFix);
		starSensor->SetUserData((void*)DATA_HEART);
		starTexture = app->tex->Load("Assets/sprites/star.png");
		starPb->listener = this;
		
	}




	return ret;
}
void Star::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_PLAYER)
	{
		LOG("Touching Star--------------");
		app->player->CollectStar();
		setToDestroy = true;

	}
}
bool Star::PreUpdate()
{
	if (setToDestroy) Die();
	return true;
}
bool Star::Update(float dt)
{
	return true;
}
bool Star::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive)
	{
		app->render->DrawTexture(starTexture, METERS_TO_PIXELS(starPb->body->GetPosition().x) - 20, METERS_TO_PIXELS(starPb->body->GetPosition().y) - 20);
	}

	return true;
}
void Star::Die()
{
	app->physics->world->DestroyBody(starPb->body);
	setToDestroy = false;
	isAlive = false;
}
bool Star::CleanUp()
{
	return true;
}
bool Star::LoadState(pugi::xml_node& node)
{
	if (isAlive)
	{
		position.x = node.child("position").attribute("x").as_int();
		position.y = node.child("position").attribute("y").as_int();

		starPb->body->SetTransform({ (float)position.x,(float)position.y }, 0);


	}
	else
	{
		isAlive = node.child("is_alive").attribute("value").as_bool();
		if (isAlive)
		{

			position.x = node.child("position").attribute("x").as_int();
			position.y = node.child("position").attribute("y").as_int();

			Start();
			starPb->body->SetTransform({ (float)position.x,(float)position.y }, 0);


		}
	}
	return true;
}
bool Star::SaveState(pugi::xml_node& node)
{
	pugi::xml_node alive = node.append_child("is_alive");
	alive.append_attribute("value").set_value(isAlive);
	if (isAlive)
	{
		pugi::xml_node pos = node.append_child("position");

		pos.append_attribute("x").set_value(starPb->body->GetPosition().x);
		pos.append_attribute("y").set_value(starPb->body->GetPosition().y);
	}
	return true;
}
void Star::Destroy()
{

}