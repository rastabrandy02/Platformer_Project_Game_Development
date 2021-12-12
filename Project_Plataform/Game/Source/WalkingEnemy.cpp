#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "WalkingEnemy.h"


WalkingEnemy::WalkingEnemy(int x, int y)
{
	idleAnimationRight.PushBack({ 12, 7, 80, 95 });
	idleAnimationRight.PushBack({ 102,7,80,95 });
	idleAnimationRight.PushBack({ 195,7,80,95 });
	idleAnimationRight.PushBack({ 276,7,80,95 });
	idleAnimationRight.loop = true;
	idleAnimationRight.speed = 0.04f;
	currentAnimation = &idleAnimationRight;

	idleAnimationLeft.PushBack({ 366,7,80,95 });
	idleAnimationLeft.PushBack({ 454,7,80,95 });
	idleAnimationLeft.PushBack({ 545,7,80,95 });
	idleAnimationLeft.PushBack({ 637,7,80,95 });
	idleAnimationLeft.speed = 0.04f;
	idleAnimationLeft.loop = true;


	runAnimationRight.PushBack({ 12, 7, 80, 95 });
	runAnimationRight.PushBack({ 102,7,80,95 });
	runAnimationRight.PushBack({ 195,7,80,95 });
	runAnimationRight.PushBack({ 276,7,80,95 });
	runAnimationRight.speed = 0.09f;
	runAnimationRight.loop = true;

	runAnimationLeft.PushBack({ 366,7,80,95 });
	runAnimationLeft.PushBack({ 454,7,80,95 });
	runAnimationLeft.PushBack({ 545,7,80,95 });
	runAnimationLeft.PushBack({ 637,7,80,95 });
	runAnimationLeft.speed = 0.09f;
	runAnimationLeft.loop = true;


	jumpAnimationRight.PushBack({ 12, 192, 80, 95 });
	jumpAnimationRight.PushBack({ 104, 192, 80, 95 });
	jumpAnimationRight.PushBack({ 195, 192, 80, 95 });
	jumpAnimationRight.PushBack({ 277, 192, 80, 95 });
	jumpAnimationRight.speed = 0.09f;
	jumpAnimationRight.loop = true;

	jumpAnimationLeft.PushBack({ 366,192,80,95 });
	jumpAnimationLeft.PushBack({ 455,192,80,95 });
	jumpAnimationLeft.PushBack({ 547,192,80,95 });
	jumpAnimationLeft.PushBack({ 636,192,80,95 });
	jumpAnimationLeft.loop = true;

	landAnimationRight.PushBack({ 12, 95, 80, 85 });
	landAnimationRight.speed = 0.00009f;
	landAnimationRight.loop = false;

	landAnimationLeft.PushBack({ 546,95,80,85 });
	landAnimationLeft.speed = 0.09f;
	landAnimationLeft.loop = false;


	dieAnimationRight.PushBack({ 15,280,80,85 });
	dieAnimationRight.PushBack({ 110,280,80,85 });
	dieAnimationRight.PushBack({ 200,280,80,85 });
	dieAnimationRight.PushBack({ 290,280,80,85 });
	dieAnimationRight.speed = 0.07f;
	dieAnimationRight.loop = false;

	dieAnimationLeft.PushBack({ 370,280,80,85 });
	dieAnimationLeft.PushBack({ 405,280,80,85 });
	dieAnimationLeft.PushBack({ 542,280,80,85 });
	dieAnimationLeft.PushBack({ 637,280,80,85 });
	dieAnimationLeft.speed = 0.07f;
	dieAnimationLeft.loop = false;
	position.x = x;
	position.y = y;

	pathfinding = new PathFinding();
	name.Create("walking_enemy");
	
}
WalkingEnemy::~WalkingEnemy()
{


}

// Load assets
bool WalkingEnemy::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemy");

	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();

	return true;
}
bool WalkingEnemy::Start()
{
	bool ret = true;

	walkingEnemy = app->tex->Load("Assets/sprites/walkingenemy_spritesheet_extended.png");

	if (app->currentScene == SCENE_LEVEL_1)
	{

		//enemy = app->physics->CreateCircle(200, 200, 25);
		enemy = app->physics->CreateCircle(position.x, position.y, 25);


		enemy->body->SetFixedRotation(true);
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
bool WalkingEnemy::PreUpdate()
{
	if (setToDestroy)Die();
	aggro = CheckAggro();

	return true;
}

bool WalkingEnemy::Update(float dt)
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive && aggro)
	{
		Path();
		Move();
	}
	if (aggro == false)
	{
		enemy->body->SetLinearVelocity({ 0, 0 });
	}
	currentAnimation->Update();
	return true;
}

bool WalkingEnemy::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive)
	{
		SDL_Rect section = currentAnimation->GetCurrentFrame();

		app->render->DrawTexture(walkingEnemy, METERS_TO_PIXELS(enemy->body->GetPosition().x) - 35, METERS_TO_PIXELS(enemy->body->GetPosition().y) - 50, &section);

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
void WalkingEnemy::Path()
{
	iPoint origin = { (int)METERS_TO_PIXELS((int)enemy->body->GetPosition().x), (int)METERS_TO_PIXELS((int)enemy->body->GetPosition().y) };
	iPoint destination = { (int)METERS_TO_PIXELS((int)app->player->player->body->GetPosition().x), (int)METERS_TO_PIXELS((int)app->player->player->body->GetPosition().y) };

	origin = app->map->WorldToMap(origin.x, origin.y);
	destination = app->map->WorldToMap(destination.x, destination.y);

	pathfinding->CreatePath(origin, destination);
	currentPath = pathfinding->GetLastPath();
}
void WalkingEnemy::Move()
{
	

	iPoint nextStep = { currentPath->At(1)->x, currentPath->At(1)->y};
	nextStep = app->map->MapToWorld(nextStep.x, nextStep.y);
	nextStep.x = PIXEL_TO_METERS(nextStep.x);
	nextStep.y = PIXEL_TO_METERS(nextStep.y);


	if (enemy->body->GetPosition().x >= nextStep.x)
	{
		enemy->body->SetLinearVelocity({ -speed.x ,enemy->body->GetLinearVelocity().y });
		currentAnimation = &runAnimationLeft;
	}
	if (enemy->body->GetPosition().x < nextStep.x)
	{
		enemy->body->SetLinearVelocity({ speed.x,enemy->body->GetLinearVelocity().y });
		currentAnimation = &runAnimationRight;
	}
	if (canJump)
	{
		if (enemy->body->GetPosition().y > nextStep.y + 1)
		{
			enemy->body->SetLinearVelocity({ enemy->body->GetLinearVelocity().x , speed.y });
			switch (lookingAt)
			{
			case RIGHT:
			{
				currentAnimation = &jumpAnimationRight;
			}break;
			case LEFT:
			{
				currentAnimation = &jumpAnimationLeft;
			}
			default:
				break;

				canJump = false;
			}
		}
	}
	

	enemyRec.x = METERS_TO_PIXELS(enemy->body->GetPosition().x) - size/2;
	enemyRec.y = METERS_TO_PIXELS(enemy->body->GetPosition().y) - size/2;
}

void WalkingEnemy::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_PLAYER || bodyA->body->GetFixtureList()->GetUserData() == (void*)DATA_PLAYER)
	{
		app->player->TakeDamage(1);
		LOG("Enemy Collision----------");
		
	}
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_GROUND)
	{
		canJump = true;
		LOG("Enemy On The Ground----------");
	}
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_FIREBALL)
	{

		LOG("Walking Enemy Collision With Fireball----------");
		health -= 2;
		if (health <= 0) setToDestroy = true;

		if (isDead)
		{
			if (enemy->body->GetPosition().x)
			{
				enemy->body->SetLinearVelocity({ -speed.x ,enemy->body->GetLinearVelocity().y });
				currentAnimation = &dieAnimationLeft;
			}
			if (enemy->body->GetPosition().x)
			{
				enemy->body->SetLinearVelocity({ speed.x,enemy->body->GetLinearVelocity().y });
				currentAnimation = &dieAnimationRight;
			}
		}
	}
}
bool WalkingEnemy::CheckAggro()
{
	uint dist = abs(sqrt(pow(enemy->body->GetPosition().x - app->player->player->body->GetPosition().x, 2) +
		pow(enemy->body->GetPosition().y - app->player->player->body->GetPosition().y, 2)));
	if (dist < aggroDistance) return true;
	if (dist > aggroDistance) return false;
}
bool WalkingEnemy::LoadState(pugi::xml_node& node)
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
bool WalkingEnemy::SaveState(pugi::xml_node& node)
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

bool WalkingEnemy::CleanUp()
{
	return true;
}
void WalkingEnemy::Die()
{
	app->physics->world->DestroyBody(enemy->body);
	setToDestroy = false;
	isAlive = false;
}