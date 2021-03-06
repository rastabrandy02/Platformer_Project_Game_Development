#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "FlyingEnemy.h"


FlyingEnemy::FlyingEnemy(int x, int y)
{
	
	idleAnimationRight.PushBack({ 60, 15, 130, 95 });
	idleAnimationRight.PushBack({ 205,15,130,95 });
	idleAnimationRight.PushBack({ 333,15,130,95 });
	idleAnimationRight.PushBack({ 468,15,130,95 });
	idleAnimationRight.PushBack({ 620,15,130,95 });
	idleAnimationRight.loop = true;
	idleAnimationRight.speed = 0.04f;
	currentAnimation = &idleAnimationRight;

	
	idleAnimationLeft.PushBack({ 60,350,130,95 });
	idleAnimationLeft.PushBack({ 205,350,130,95 });
	idleAnimationLeft.PushBack({ 360,350,130,95 });
	idleAnimationLeft.PushBack({ 490,350,130,95 });
	idleAnimationLeft.PushBack({ 620,350,130,95 });
	idleAnimationLeft.speed = 0.04f;
	idleAnimationLeft.loop = true;

	
	dieAnimationRight.PushBack({ 60, 232, 130, 95 });
	dieAnimationRight.PushBack({ 203,232,130,95 });
	dieAnimationRight.PushBack({ 335,232,130,95 });
	dieAnimationRight.PushBack({ 470,232,130,95 });
	dieAnimationRight.PushBack({ 620,232,130,95 });
	dieAnimationRight.speed = 0.07f;
	dieAnimationRight.loop = false;
	
	dieAnimationLeft.PushBack({ 60,587,130,95 });
	dieAnimationLeft.PushBack({ 205,587,130,95 });
	dieAnimationLeft.PushBack({ 362,587,130,95 });
	dieAnimationLeft.PushBack({ 492,587,130,95 });
	dieAnimationLeft.PushBack({ 620,587,130,95 });
	dieAnimationLeft.speed = 0.07f;
	dieAnimationLeft.loop = false;

	
	flyAnimationRight.PushBack({ 60, 15, 130, 95 });
	flyAnimationRight.PushBack({ 205, 15, 130, 95 });
	flyAnimationRight.PushBack({ 333, 15, 130, 95 });
	flyAnimationRight.PushBack({ 468, 15, 130, 95 });
	flyAnimationRight.PushBack({ 620, 15, 130, 95 });
	flyAnimationRight.speed = 0.04f;
	flyAnimationRight.loop = true;
	
	flyAnimationLeft.PushBack({ 60, 350, 130, 95});
	flyAnimationLeft.PushBack({ 205, 350, 130, 95 });
	flyAnimationLeft.PushBack({ 360, 350, 130, 95 });
	flyAnimationLeft.PushBack({ 490, 350, 130, 95 });
	flyAnimationLeft.PushBack({ 620, 350, 130, 95 });
	flyAnimationLeft.speed = 0.04f;
	flyAnimationLeft.loop = true;

	position.x = x;
	position.y = y;

	pathfinding = new PathFinding();
	name.Create("flying_enemy");
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
		enemy = app->physics->CreateCircle(position.x, position.y, 15);

		enemy->body->SetFixedRotation(true);
		enemy->body->SetType(b2_kinematicBody);
		enemy->body->GetFixtureList()->SetFriction(5.0f);

		b2PolygonShape sensorShape;
		
		sensorShape.SetAsBox(PIXEL_TO_METERS(size/2), PIXEL_TO_METERS(size/2));


		b2FixtureDef sensorFix;
		sensorFix.shape = &sensorShape;
		sensorFix.isSensor = true;

		enemySensor = enemy->body->CreateFixture(&sensorFix);
		enemySensor->SetUserData((void*)DATA_ENEMY);
		enemyRec = { METERS_TO_PIXELS((int)enemy->body->GetPosition().x) - size,METERS_TO_PIXELS((int)enemy->body->GetPosition().y) + size, 60,60 };
		enemy->listener = this;

		
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
	if (setToDestroy) Die();

	aggro = CheckAggro();

	return true;
}
bool FlyingEnemy::Update(float dt)
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive && aggro)
	{
		Path();
		Move();

	}
	if (aggro == false)
	{
		enemy->body->SetLinearVelocity({0, 0});
	}
	currentAnimation->Update();
	return true;
}

bool FlyingEnemy::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive)
	{
		SDL_Rect section = currentAnimation->GetCurrentFrame();

		app->render->DrawTexture(flyingEnemy, METERS_TO_PIXELS(enemy->body->GetPosition().x) - 60, METERS_TO_PIXELS(enemy->body->GetPosition().y) - 60, &section);
		
		if (aggro && app->physics->debug)
		{
				for (uint i = 0; i < currentPath->Count(); ++i)
				{
					iPoint pos = app->map->MapToWorld(currentPath->At(i)->x, currentPath->At(i)->y);
					app->render->DrawTexture(navigationPath, pos.x, pos.y);
				}
		}
		
	}
	return true;
}
void FlyingEnemy::Path()
{
	iPoint origin = { (int)METERS_TO_PIXELS((int)enemy->body->GetPosition().x), (int)METERS_TO_PIXELS((int)enemy->body->GetPosition().y) };
	iPoint destination = { (int)METERS_TO_PIXELS((int)app->player->player->body->GetPosition().x), (int)METERS_TO_PIXELS((int)app->player->player->body->GetPosition().y) };

	origin = app->map->WorldToMap(origin.x, origin.y);
	destination = app->map->WorldToMap(destination.x, destination.y);

	pathfinding->CreatePath(origin, destination);
	currentPath = pathfinding->GetLastPath();
}
void FlyingEnemy::Move()
{
	

	iPoint nextStep = { currentPath->At(1)->x, currentPath->At(1)->y };
	nextStep = app->map->MapToWorld(nextStep.x, nextStep.y);
	nextStep.x = PIXEL_TO_METERS(nextStep.x);
	nextStep.y = PIXEL_TO_METERS(nextStep.y);

	int posX = enemy->body->GetPosition().x;
	int posY = enemy->body->GetPosition().y;
	if (posX >= nextStep.x)
	{
		enemy->body->SetLinearVelocity({ -speed.x ,enemy->body->GetLinearVelocity().y });
		currentAnimation = &flyAnimationLeft;
	}
	if (posX < nextStep.x)
	{
		enemy->body->SetLinearVelocity({ speed.x,enemy->body->GetLinearVelocity().y });
		currentAnimation = &flyAnimationRight;
	}

	
	if (posY < nextStep.y)
	{
		enemy->body->SetLinearVelocity({ enemy->body->GetLinearVelocity().x , -speed.y });

	}
	if (posY > nextStep.y)
	{
		enemy->body->SetLinearVelocity({ enemy->body->GetLinearVelocity().x , speed.y });

	}
	if (posY == nextStep.y) enemy->body->SetLinearVelocity({ enemy->body->GetLinearVelocity().x, 0 });
	if (posX == nextStep.x) enemy->body->SetLinearVelocity({ 0, enemy->body->GetLinearVelocity().y });

	enemyRec.x = METERS_TO_PIXELS(enemy->body->GetPosition().x) - size/2;
	enemyRec.y = METERS_TO_PIXELS(enemy->body->GetPosition().y) - size/2;
}

void FlyingEnemy::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_PLAYER || bodyA->body->GetFixtureList()->GetUserData() == (void*)DATA_PLAYER)
	{
		app->player->TakeDamage(1);
		LOG("Flying Enemy Collision----------");
		
	}
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_FIREBALL || bodyA->body->GetFixtureList()->GetUserData() == (void*)DATA_FIREBALL)
	{
		
		LOG("Flying Enemy Collision With Fireball----------");
		health -= 2;
		if (health <= 0) setToDestroy = true;
	}
	
}
bool FlyingEnemy::CheckAggro()
{
	uint dist = abs(sqrt(pow( enemy->body->GetPosition().x - app->player->player->body->GetPosition().x, 2) +
		pow(enemy->body->GetPosition().y - app->player->player->body->GetPosition().y, 2) ));
	if (dist < aggroDistance) return true;
	if (dist > aggroDistance) return false;
}
bool FlyingEnemy::LoadState(pugi::xml_node& node)
{
	if (isAlive)
	{
		position.x = node.child("position").attribute("x").as_float();
		position.y = node.child("position").attribute("y").as_float();
		enemy->body->SetTransform({ (float)position.x,(float)position.y }, 0);
		Path();
	}
	else
	{
		isAlive = node.child("is_alive").attribute("value").as_bool();
		if (isAlive)
		{

			position.x = node.child("position").attribute("x").as_float();
			position.y = node.child("position").attribute("y").as_float();

			Start();
			enemy->body->SetTransform({ (float)position.x,(float)position.y }, 0);
			Path();
		}
	}
	
	return true;
}
bool FlyingEnemy::SaveState(pugi::xml_node& node)
{
	pugi::xml_node alive = node.append_child("is_alive");
	alive.append_attribute("value").set_value(isAlive);
	if (isAlive)
	{
		pugi::xml_node pos = node.append_child("position");
		pos.append_attribute("x").set_value(enemy->body->GetPosition().x);
		pos.append_attribute("y").set_value(enemy->body->GetPosition().y + 1);
	}
	return true;
}

bool FlyingEnemy::CleanUp()
{
	return true;
}
void FlyingEnemy::Die()
{
	app->physics->world->DestroyBody(enemy->body);
	setToDestroy = false;
	isAlive = false;
}