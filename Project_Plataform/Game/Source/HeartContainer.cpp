#include "App.h"
#include "HeartContainer.h"


HeartContainer::HeartContainer(int x, int y)
{
	heartAnimation.PushBack({ 37,30,38,38 });
	heartAnimation.PushBack({ 75,30,38,38 });
	heartAnimation.PushBack({ 130,30,38,38 });
	heartAnimation.loop = true;
	heartAnimation.speed = 0.04f;
	currentAnimation = &heartAnimation;
	position.x = x;
	position.y = y;
	name.Create("heart_container");
}
HeartContainer::~HeartContainer()
{

}

bool HeartContainer::Awake(pugi::xml_node& config)
{
	LOG("Loading Hearts");
	

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
		heartSensor->SetUserData((void*)DATA_HEART);
		heartTexture = app->tex->Load("Assets/sprites/heart.png");
		heartPb->listener = this;

		setToDestroy = false;
		isAlive = true;
	}




	return ret;
}
void HeartContainer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_PLAYER)
	{
		LOG("Touching Heart--------------");
		app->player->Heal(5);
		setToDestroy = true;
		


	}
}
bool HeartContainer::PreUpdate()
{
	if(setToDestroy) Die();
	return true;
}
bool HeartContainer::Update(float dt)
{
	return true;
}
bool HeartContainer::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive)
	{
		app->render->DrawTexture(heartTexture, METERS_TO_PIXELS(heartPb->body->GetPosition().x) -30, METERS_TO_PIXELS(heartPb->body->GetPosition().y) -30);
		SDL_Rect section = currentAnimation->GetCurrentFrame();
	}

	return true;
}
void HeartContainer::Die()
{
	app->physics->world->DestroyBody(heartPb->body);
	setToDestroy = false;
	isAlive = false;
}
bool HeartContainer::CleanUp()
{
	return true;
}
bool HeartContainer::LoadState(pugi::xml_node& node)
{
	if (isAlive)
	{
		position.x = node.child("position").attribute("x").as_int();
		position.y = node.child("position").attribute("y").as_int();
		
		heartPb->body->SetTransform({ (float)position.x,(float)position.y }, 0);
		
		
	}
	else
	{
		isAlive = node.child("is_alive").attribute("value").as_bool();
		if (isAlive)
		{

			position.x = node.child("position").attribute("x").as_int();
			position.y = node.child("position").attribute("y").as_int();

			Start();
			heartPb->body->SetTransform({ (float)position.x,(float)position.y }, 0);
			
			
		}
	}
	return true;
}
bool HeartContainer::SaveState(pugi::xml_node& node)
{
	pugi::xml_node alive = node.append_child("is_alive");
	alive.append_attribute("value").set_value(isAlive);
	if (isAlive)
	{
		pugi::xml_node pos = node.append_child("position");
		
		pos.append_attribute("x").set_value(heartPb->body->GetPosition().x);
		pos.append_attribute("y").set_value(heartPb->body->GetPosition().y);
	}
	
	return true;
}
void HeartContainer::Destroy()
{

}