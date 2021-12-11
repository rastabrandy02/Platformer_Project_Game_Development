#include "App.h"
#include "HeartContainer.h"


HeartContainer::HeartContainer()
{
	heartAnimation.PushBack({ 37,30,38,38 });
	heartAnimation.PushBack({ 75,30,38,38 });
	heartAnimation.PushBack({ 130,30,38,38 });
	heartAnimation.loop = true;
	heartAnimation.speed = 0.04f;
	currentAnimation = &heartAnimation;
}
HeartContainer::~HeartContainer()
{

}

bool HeartContainer::Awake(pugi::xml_node& config)
{
	LOG("Loading Hearts");
	position.x = 700;
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
		heartSensor->SetUserData((void*)DATA_HEART);
		heartTexture = app->tex->Load("Assets/sprites/heart.png");
		heartPb->listener = this;
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
	if (app->currentScene == SCENE_LEVEL_1 && draw)
	{
		app->render->DrawTexture(heartTexture, position.x - 30, position.y + 70);
		SDL_Rect section = currentAnimation->GetCurrentFrame();
	}

	return true;
}
void HeartContainer::Die()
{
	app->physics->world->DestroyBody(heartPb->body);
	setToDestroy = false;
	draw = false;
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
void HeartContainer::Destroy()
{

}