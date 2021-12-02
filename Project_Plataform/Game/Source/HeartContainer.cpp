#include "App.h"
#include "HeartContainer.h"

HeartContainer::HeartContainer()
{

}
HeartContainer::~HeartContainer()
{

}
bool HeartContainer::Awake(pugi::xml_node& config)
{
	LOG("Loading Hearts");

	/*position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();*/
	position.x = 500;
	position.y = 200;

	return true;
}
bool HeartContainer::Start()
{
	bool ret = true;



	if (app->currentScene == SCENE_LEVEL_1)
	{
		heartPb = app->physics->CreateCircle(position.x, position.y, 25);

		heartPb->body->SetFixedRotation(true);
		heartPb->body->GetFixtureList()->SetFriction(5.0f);

		b2PolygonShape sensorShape;
		sensorShape.SetAsBox(PIXEL_TO_METERS(30), PIXEL_TO_METERS(30));


		b2FixtureDef sensorFix;
		sensorFix.shape = &sensorShape;
		sensorFix.isSensor = true;

		heartSensor = heartPb->body->CreateFixture(&sensorFix);
		heartSensor->SetUserData((void*)DATA_ENEMY);
		heartTexture = app->tex->Load("Assets/sprites/heart_spritesheet.png");
		heartPb->listener = app->heartcontainer;
	}




	return ret;
}
void HeartContainer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_PLAYER)
	{
		LOG("Touching Heart--------------");
	}
}
bool HeartContainer::PreUpdate()
{
	return true;
}
bool HeartContainer::Update(float dt)
{
	return true;
}
bool HeartContainer::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1)
	{
		app->render->DrawTexture(heartTexture, position.x, position.y);
	}
	
	return true;
}
void HeartContainer::Die()
{

}
bool HeartContainer::CleanUp()
{
	return true;
}
bool HeartContainer::LoadState(pugi::xml_node&)
{
	return true;
}
bool HeartContainer::SaveState(pugi::xml_node&)
{
	return true;
}