#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "FlyingEnemy.h"

FlyingEnemy::FlyingEnemy(): Module()
{
	//animations

	idleAnimationRight.PushBack({ 10, 6, 85, 95 });
	idleAnimationRight.PushBack({ 85,6,85,95 });
	idleAnimationRight.PushBack({ 167,6,85,95 });
	idleAnimationRight.PushBack({ 248,6,85,95 });
	idleAnimationRight.loop = true;
	idleAnimationRight.speed = 0.04f;
	currentAnimation = &idleAnimationRight;


	idleAnimationLeft.PushBack({ 330,6,85,95 });
	idleAnimationLeft.PushBack({ 405,6,85,95 });
	idleAnimationLeft.PushBack({ 485,6,85,95 });
	idleAnimationLeft.PushBack({ 566,6,85,95 });
	idleAnimationLeft.speed = 0.04f;
	idleAnimationLeft.loop = true;

	

	dieAnimationRight.PushBack({ 20,280,85,85 });
	dieAnimationRight.PushBack({ 110,280,85,85 });
	dieAnimationRight.PushBack({ 200,280,85,85 });
	dieAnimationRight.PushBack({ 290,280,75,85 });
	dieAnimationRight.speed = 0.07f;
	dieAnimationRight.loop = false;

	dieAnimationLeft.PushBack({ 625,280,85,85 });
	dieAnimationLeft.PushBack({ 535,280,85,85 });
	dieAnimationLeft.PushBack({ 440,280,85,85 });
	dieAnimationLeft.PushBack({ 365,280,75,85 });
	dieAnimationLeft.speed = 0.07f;
	dieAnimationLeft.loop = false;

	position.x = 500;
	position.y = 300;
	name.Create("Walking_Enemy");
}

FlyingEnemy::FlyingEnemy(int x, int y)
{
	//animations

	idleAnimationRight.PushBack({ 10, 6, 85, 95 });
	idleAnimationRight.PushBack({ 85,6,85,95 });
	idleAnimationRight.PushBack({ 167,6,85,95 });
	idleAnimationRight.PushBack({ 248,6,85,95 });
	idleAnimationRight.loop = true;
	idleAnimationRight.speed = 0.04f;
	currentAnimation = &idleAnimationRight;


	idleAnimationLeft.PushBack({ 330,6,85,95 });
	idleAnimationLeft.PushBack({ 405,6,85,95 });
	idleAnimationLeft.PushBack({ 485,6,85,95 });
	idleAnimationLeft.PushBack({ 566,6,85,95 });
	idleAnimationLeft.speed = 0.04f;
	idleAnimationLeft.loop = true;



	dieAnimationRight.PushBack({ 20,280,85,85 });
	dieAnimationRight.PushBack({ 110,280,85,85 });
	dieAnimationRight.PushBack({ 200,280,85,85 });
	dieAnimationRight.PushBack({ 290,280,75,85 });
	dieAnimationRight.speed = 0.07f;
	dieAnimationRight.loop = false;

	dieAnimationLeft.PushBack({ 625,280,85,85 });
	dieAnimationLeft.PushBack({ 535,280,85,85 });
	dieAnimationLeft.PushBack({ 440,280,85,85 });
	dieAnimationLeft.PushBack({ 365,280,75,85 });
	dieAnimationLeft.speed = 0.07f;
	dieAnimationLeft.loop = false;

	position.x = x;
	position.y = y;
	name.Create("Walking_Enemy");
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
		enemy->listener = app->flyingenemy;

		pathfinding = new PathFinding();
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
<<<<<<< Updated upstream


=======
	aggro = CheckAggro();
>>>>>>> Stashed changes
	return true;
}
bool FlyingEnemy::Update(float dt)
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive && aggro)
	{
		iPoint origin = { (int)METERS_TO_PIXELS((int)enemy->body->GetPosition().x), (int)METERS_TO_PIXELS((int)enemy->body->GetPosition().y) };
		iPoint destination = { (int)METERS_TO_PIXELS((int)app->player->player->body->GetPosition().x), (int)METERS_TO_PIXELS((int)app->player->player->body->GetPosition().y) };

		origin = app->map->WorldToMap(origin.x, origin.y);
		destination = app->map->WorldToMap(destination.x, destination.y);

		pathfinding->CreatePath(origin, destination);
		currentPath = pathfinding->GetLastPath();
		Move();

	}
	if (aggro == false)
	{
		enemy->body->SetLinearVelocity({0, 0});
	}
	return true;
}

bool FlyingEnemy::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1 && isAlive)
	{
		SDL_Rect section = currentAnimation->GetCurrentFrame();
		//app->render->DrawTexture(walkingEnemy, 255, 0, 0, 255);
<<<<<<< Updated upstream
		//app->render->DrawRectangle({ METERS_TO_PIXELS(enemy->body->GetPosition().x) - size/2,METERS_TO_PIXELS(enemy->body->GetPosition().y) - size/2, size,size }, 0, 255, 0, 255);
		app->render->DrawTexture(flyingEnemy, METERS_TO_PIXELS(enemy->body->GetPosition().x) - 35, METERS_TO_PIXELS(enemy->body->GetPosition().y) - 50, &section);


		for (uint i = 0; i < currentPath->Count(); ++i)
=======
		app->render->DrawRectangle({ METERS_TO_PIXELS(enemy->body->GetPosition().x) - size/2,METERS_TO_PIXELS(enemy->body->GetPosition().y) - size/2, size,size }, 0, 255, 0, 255);
		if (aggro && app->physics->debug)
>>>>>>> Stashed changes
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
	}
	if (posX < nextStep.x)
	{
		enemy->body->SetLinearVelocity({ speed.x,enemy->body->GetLinearVelocity().y });
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

	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_PLAYER)
	{
		app->player->TakeDamage(1);
		LOG("Flying Enemy Collision----------");
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
bool FlyingEnemy::LoadState(pugi::xml_node&)
{
	return true;
}
bool FlyingEnemy::SaveState(pugi::xml_node&)
{
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