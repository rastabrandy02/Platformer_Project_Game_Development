#include "App.h"
#include "Star.h"

Star::Star()
{
	
}
Star::~Star()
{

}

bool Star::Awake(pugi::xml_node& config)
{
	LOG("Loading Hearts");

	position.x = 1090;
	position.y = 200;

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
	if (app->currentScene == SCENE_LEVEL_1 && draw)
	{
		app->render->DrawTexture(starTexture, position.x - 20, position.y - 20);
	}

	return true;
}
void Star::Die()
{
	app->physics->world->DestroyBody(starPb->body);
	setToDestroy = false;
	draw = false;
}
bool Star::CleanUp()
{
	return true;
}
bool Star::LoadState(pugi::xml_node&)
{
	return true;
}
bool Star::SaveState(pugi::xml_node&)
{
	return true;
}
void Star::Destroy()
{

}