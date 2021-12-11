#include "App.h"
#include "Checkpoint.h"

Checkpoint::Checkpoint()
{

}
Checkpoint::~Checkpoint()
{

}
bool Checkpoint::Awake(pugi::xml_node& config)
{
	LOG("Loading Checkpoint");

	/*position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();*/
	position.x = 1500;
	position.y = 300;

	return true;
}
bool Checkpoint::Start()
{
	bool ret = true;



	if (app->currentScene == SCENE_LEVEL_1)
	{
		checkpointPb = app->physics->CreateCircle(position.x, position.y, 25);

		checkpointPb->body->SetFixedRotation(true);
		checkpointPb->body->GetFixtureList()->SetFriction(5.0f);

		b2PolygonShape sensorShape;
		sensorShape.SetAsBox(PIXEL_TO_METERS(30), PIXEL_TO_METERS(30));


		b2FixtureDef sensorFix;
		sensorFix.shape = &sensorShape;
		sensorFix.isSensor = true;

		checkpointSensor = checkpointPb->body->CreateFixture(&sensorFix);
		checkpointSensor->SetUserData((void*)DATA_HEART);
		checkpointTexture1 = app->tex->Load("Assets/sprites/checkpoint1.png");
		checkpointTexture2 = app->tex->Load("Assets/sprites/checkpoint2.png");
		checkpointPb->listener = this;

	}




	return ret;
}
void Checkpoint::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_PLAYER)
	{
		if (!activated && isAlive)
		{
			LOG("Checkpoint reached!--------------");
			app->SaveGameRequest();
			activated = true;
		}
		



	}
}
bool Checkpoint::PreUpdate()
{
	if (activated && isAlive) Die();
	return true;
}
bool Checkpoint::Update(float dt)
{
	return true;
}
bool Checkpoint::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1)
	{
		//app->render->DrawTexture(checkpointTexture, position.x, position.y);
		if (!activated)
		{
			//app->render->DrawRectangle({ position.x - 20, position.y - 20, 40,40 }, 0, 0, 255, 255);
			app->render->DrawTexture(checkpointTexture1, position.x - 30, position.y - 40);

		}
		if (activated)
		{
			//app->render->DrawRectangle({ position.x - 20, position.y - 20, 40,40 }, 0, 255, 0, 255);
			app->render->DrawTexture(checkpointTexture2, position.x - 30, position.y - 40);

		}
	}

	return true;
}
void Checkpoint::Die()
{
	app->physics->world->DestroyBody(checkpointPb->body);
	//activated = false;
	isAlive = false;
}
bool Checkpoint::CleanUp()
{
	return true;
}
bool Checkpoint::LoadState(pugi::xml_node&)
{
	return true;
}
bool Checkpoint::SaveState(pugi::xml_node&)
{
	return true;
}
void Checkpoint::Destroy()
{

}