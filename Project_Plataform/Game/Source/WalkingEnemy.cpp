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
	idleAnimationRight.PushBack({ 10, 6, 85, 95 });
	idleAnimationRight.PushBack({ 95,6,85,95 });
	idleAnimationRight.PushBack({ 188,6,85,95 });
	idleAnimationRight.PushBack({ 282,6,85,95 });
	idleAnimationRight.loop = true;
	idleAnimationRight.speed = 0.04f;
	currentAnimation = &idleAnimationRight;

	idleAnimationLeft.PushBack({ 374,6,85,95 });
	idleAnimationLeft.PushBack({ 463,6,85,95 });
	idleAnimationLeft.PushBack({ 553,6,85,95 });
	idleAnimationLeft.PushBack({ 647,6,85,95 });
	idleAnimationLeft.speed = 0.04f;
	idleAnimationLeft.loop = true;


	runAnimationRight.PushBack({ 10, 192, 85, 95 });
	runAnimationRight.PushBack({ 95,192,85,95 });
	runAnimationRight.PushBack({ 188,192,85,95 });
	runAnimationRight.PushBack({ 282,192,85,95 });
	runAnimationRight.speed = 0.09f;
	runAnimationRight.loop = true;

	runAnimationLeft.PushBack({ 374,192,85,95 });
	runAnimationLeft.PushBack({ 463,192,85,95 });
	runAnimationLeft.PushBack({ 553,192,85,95 });
	runAnimationLeft.PushBack({ 647,192,85,95 });
	runAnimationLeft.speed = 0.09f;
	runAnimationLeft.loop = true;


	jumpAnimationRight.PushBack({ 10, 100, 85, 95 });
	jumpAnimationRight.PushBack({ 100, 100, 85, 95 });
	jumpAnimationRight.speed = 0.09f;
	jumpAnimationRight.loop = true;

	jumpAnimationLeft.PushBack({ 568,100,85,95 });
	jumpAnimationLeft.PushBack({ 637,100,85,95 });
	jumpAnimationLeft.speed = 0.09f;
	jumpAnimationLeft.loop = true;

	//NEEDS THE SPRITES FOR LANDING
	landAnimationRight.PushBack({ 10, 105, 85, 85 });
	landAnimationRight.speed = 0.00009f;
	landAnimationRight.loop = false;

	landAnimationLeft.PushBack({ 556,105,85,85 });
	landAnimationLeft.speed = 0.09f;
	landAnimationLeft.loop = false;


	dieAnimationRight.PushBack({ 15,280,85,85 });
	dieAnimationRight.PushBack({ 110,280,85,85 });
	dieAnimationRight.PushBack({ 200,280,85,85 });
	dieAnimationRight.PushBack({ 290,280,75,85 });
	dieAnimationRight.speed = 0.07f;
	dieAnimationRight.loop = false;

	dieAnimationLeft.PushBack({ 372,280,85,85 });
	dieAnimationLeft.PushBack({ 450,280,85,85 });
	dieAnimationLeft.PushBack({ 542,280,85,85 });
	dieAnimationLeft.PushBack({ 640,280,75,85 });
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
	if (setToDestroy) Die();


	if (app->currentScene == SCENE_LEVEL_1)
	{
		if (lookingAt = RIGHT)
		{
			if (!onTheAir)currentAnimation = &runAnimationRight;
			if (onTheAir && !countLanding) currentAnimation = &jumpAnimationRight;
		}
		if (lookingAt = LEFT)
		{
			if (!onTheAir)currentAnimation = &runAnimationLeft;
			if (onTheAir && !countLanding) currentAnimation = &jumpAnimationLeft;
		}
	}


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
	return true;
}

bool WalkingEnemy::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive)
	{
		SDL_Rect section = currentAnimation->GetCurrentFrame();
		//app->render->DrawTexture(walkingEnemy, 255, 0, 0, 255);

		//app->render->DrawRectangle({ METERS_TO_PIXELS(enemy->body->GetPosition().x) -size/2,METERS_TO_PIXELS (enemy->body->GetPosition().y) - size/2, 60,60 }, 255, 0, 0, 255);
		app->render->DrawTexture(walkingEnemy, METERS_TO_PIXELS(enemy->body->GetPosition().x) - 35, METERS_TO_PIXELS(enemy->body->GetPosition().y) - 50, &section);

		//app->render->DrawRectangle({ METERS_TO_PIXELS(enemy->body->GetPosition().x) -size/2,METERS_TO_PIXELS( enemy->body->GetPosition().y) - size/2, 60,60 }, 255, 0, 0, 255);
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
	}
	if (enemy->body->GetPosition().x < nextStep.x)
	{
		enemy->body->SetLinearVelocity({ speed.x,enemy->body->GetLinearVelocity().y });
	}
	if (canJump)
	{
		if (enemy->body->GetPosition().y > nextStep.y + 1)
		{
			enemy->body->SetLinearVelocity({ enemy->body->GetLinearVelocity().x , speed.y });
			canJump = false;
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
		/*health -= 4;
		if (health <= 0) setToDestroy = true;*/
		setToDestroy = true;
	}
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_GROUND)
	{
		canJump = true;
		LOG("Enemy On The Ground----------");
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