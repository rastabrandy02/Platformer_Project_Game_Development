#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"

Player::Player() :  Module()
{
	idleAnimationRight.PushBack({ 10, 6, 85, 95 });
	idleAnimationRight.PushBack({ 85,6,85,95 });
	idleAnimationRight.PushBack({ 167,6,85,95});
	idleAnimationRight.PushBack({ 248,6,85,95});
	idleAnimationRight.loop = true;
	idleAnimationRight.speed = 0.04f;
	currentAnimation = &idleAnimationRight;

	
	idleAnimationLeft.PushBack({ 330,6,85,95 });
	idleAnimationLeft.PushBack({ 405,6,85,95 });
	idleAnimationLeft.PushBack({ 485,6,85,95 });
	idleAnimationLeft.PushBack({ 566,6,85,95 });
	idleAnimationLeft.speed = 0.04f;
	idleAnimationLeft.loop = true;

	//Correct the run animation's PushBacks (currently displaying idle animation)
	runAnimationRight.PushBack({ 10, 8, 85, 95 });
	runAnimationRight.PushBack({ 85,8,85,95 });
	runAnimationRight.PushBack({ 167,8,85,95 });
	runAnimationRight.PushBack({ 248,8,85,95 });
	runAnimationRight.speed = 0.09f;
	runAnimationRight.loop = true;

	runAnimationLeft.PushBack({ 330,8,85,95 });
	runAnimationLeft.PushBack({ 405,8,85,95 });
	runAnimationLeft.PushBack({ 485,8,85,95 });
	runAnimationLeft.PushBack({ 566,8,85,95 });
	runAnimationLeft.speed = 0.09f;
	runAnimationLeft.loop = true;
	
	
	//jumpAnimationRight.PushBack({ 10, 100, 85, 95 });
	jumpAnimationRight.PushBack({ 100, 100, 85, 95 });
	jumpAnimationRight.speed = 0.09f;
	jumpAnimationRight.loop = true;

	jumpAnimationLeft.PushBack({ 475,100,85,95 });
	//jumpAnimationLeft.PushBack({ 556,100,85,95 });
	jumpAnimationLeft.speed = 0.0009f;
	jumpAnimationLeft.loop = true;

	landAnimationRight.PushBack({ 10, 105, 85, 85 });
	landAnimationRight.speed = 0.00009f;
	landAnimationRight.loop = false;

	landAnimationLeft.PushBack({ 556,105,85,85 });
	landAnimationLeft.speed = 0.09f;
	landAnimationLeft.loop = false;

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

	
	shootingAnimationRight.PushBack({ 15,370,85,85 });
	shootingAnimationRight.PushBack({ 93,370,85,85 });
	shootingAnimationRight.PushBack({ 203,370,85,85 });
	shootingAnimationRight.speed = 0.07f;
	shootingAnimationRight.loop = false;

	shootingAnimationLeft.PushBack({ 323,370,85,85 });
	shootingAnimationLeft.PushBack({ 447,370,85,85 });
	shootingAnimationLeft.PushBack({ 555,370,85,85 });
	shootingAnimationLeft.speed = 0.07f;
	shootingAnimationLeft.loop = false;

	name.Create("player");
}

Player::~Player()
{}

// Load assets
bool Player::Awake(pugi::xml_node &config)
{
	LOG("Loading player");
	
	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();
	
	return true;
}
bool Player::Start()
{
	bool ret = true;
	
	//wizard = app->tex->Load("Assets/sprites/wizard_spritesheet.png");
	wizard = app->tex->Load("Assets/sprites/wizard_spritesheet_extended.png");
	if (app->currentScene == SCENE_LEVEL_1)
	{
		player = app->physics->CreateCircle(position.x, position.y, 25);

		player->body->SetFixedRotation(true);
		player->body->GetFixtureList()->SetFriction(5.0f);

		b2PolygonShape sensorShape;
		sensorShape.SetAsBox(PIXEL_TO_METERS(26), PIXEL_TO_METERS(26));


		b2FixtureDef sensorFixture;
		sensorFixture.shape = &sensorShape;
		sensorFixture.isSensor = true;

		playerSensor = player->body->CreateFixture(&sensorFixture);
		playerSensor->SetUserData((void*)DATA_PLAYER);
		player->listener = app->player;
		
	}
	
	
	
	
	return ret;
}

// Unload assets
bool Player::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// PreUpdate: Imput
bool Player::PreUpdate()
{
	bool ret = true;

	if (app->currentScene == SCENE_LEVEL_1)
	{
		if (godMode) player->body->SetGravityScale(0.0f);
		else player->body->SetGravityScale(1.0f);

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !isDead)
		{

			if (!godMode)player->body->SetLinearVelocity({ speed.x, player->body->GetLinearVelocity().y });
			if (godMode) player->body->SetTransform({ player->body->GetPosition().x + PIXEL_TO_METERS(10), player->body->GetPosition().y }, 0.0f);
			lookingAt = RIGHT;
			if (!onTheAir)currentAnimation = &runAnimationRight;
			if (onTheAir && !countLanding) currentAnimation = &jumpAnimationRight;

		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !isDead)
		{


			if (!godMode)player->body->SetLinearVelocity({ -speed.x, player->body->GetLinearVelocity().y });
			if (godMode) player->body->SetTransform({ player->body->GetPosition().x - PIXEL_TO_METERS(10), player->body->GetPosition().y }, 0.0f);

			lookingAt = LEFT;
			if (!onTheAir)currentAnimation = &runAnimationLeft;
			if (onTheAir && !countLanding) currentAnimation = &jumpAnimationLeft;
		}
		
		//SHOOTING
		/*if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && !isDead)
		{

			if (!godMode)player->body->SetLinearVelocity({ speed.x, player->body->GetLinearVelocity().y });
			if (godMode) player->body->SetTransform({ player->body->GetPosition().x + PIXEL_TO_METERS(10), player->body->GetPosition().y }, 0.0f);
			lookingAt = RIGHT;
			if (!onTheAir)currentAnimation = &shootingAnimationRight;
			if (onTheAir && !countLanding) currentAnimation = &jumpAnimationRight;

			lookingAt = LEFT;
			if (!onTheAir)currentAnimation = &shootingAnimationLeft;
			if (onTheAir && !countLanding) currentAnimation = &jumpAnimationLeft;
		}
		*/

		if (canJump || canDoubleJump && !isDead && !godMode)
		{
			if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
			{

				player->body->SetLinearVelocity({ player->body->GetLinearVelocity().x , speed.y });
				onTheAir = true;


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
				}
				if (canJump == false)
				{
					canDoubleJump = false;
				}
				canJump = false;
			}
		}
		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE) && (app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) && (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) && !onTheAir && !isDead)
		{
			switch (lookingAt)
			{
			case RIGHT:
			{
				currentAnimation = &idleAnimationRight;
			}break;
			case LEFT:
			{
				currentAnimation = &idleAnimationLeft;
			}
			default:
				break;
			}
		}

		// GOD MODE
		// GOD MODE
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) godMode = !godMode;
		if (godMode)
		{


			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				player->body->SetTransform({ player->body->GetPosition().x, player->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0.0f);
			}

			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				player->body->SetTransform({ player->body->GetPosition().x, player->body->GetPosition().y + PIXEL_TO_METERS(10) }, 0.0f);
			}
		}
	}
	
	return true;
}
bool Player::Update(float dt)
{
	bool ret = true;
	
	if (app->currentScene == SCENE_LEVEL_1)
	{
		
		if (onTheAir && countLanding)
		{
			landingTimer++;
			if (landingTimer >= 30)
			{
				onTheAir = false;
				countLanding = false;
				landingTimer = 0;
			}
		}
		if (isDead)
		{
			deathTimer++;
			if (deathTimer >= 60)
			{
				deathTimer = 0;
				isDead = false;
				if(!godMode)Die();
			}
		}


		currentAnimation->Update();
	}
	
	return ret;
}
bool Player::PostUpdate()
{
	bool ret = true;
	if (app->currentScene == SCENE_LEVEL_1)
	{
		SDL_Rect section = currentAnimation->GetCurrentFrame();
		//app->render->DrawTexture(wizard, position.x,position.y, &section);
		app->render->DrawTexture(wizard, METERS_TO_PIXELS(player->body->GetPosition().x) - 35, METERS_TO_PIXELS(player->body->GetPosition().y) - 50, &section);

		int playerPosX;
		int playerPosY;
		player->GetPosition(playerPosX, playerPosY);
		if (playerPosX > app->render->camera.x + 300) app->render->camera.x = -playerPosX + 300;
	}
	
		
	return ret;
}

bool Player::LoadState(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();
	player->body->SetTransform({ (float)position.x,(float)position.y }, 0);
	return true;
}
bool Player::SaveState(pugi::xml_node& node)
{
	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x").set_value(player->body->GetPosition().x);
	pos.append_attribute("y").set_value(player->body->GetPosition().y +1);
	
	return true;
}
void Player::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB->body->GetFixtureList()->GetUserData() == (void*)DATA_ENEMY)
	{
		LOG("Touching enemy--------------------");
	}
	int otherData = (int) bodyB->body->GetFixtureList()->GetUserData();
	if (otherData ==  DATA_DEATH)
	{
		
		switch (lookingAt)
		{
		case RIGHT:
		{
			currentAnimation = &dieAnimationRight;
			isDead = true;

		}break;
		case LEFT:
		{
			currentAnimation = &dieAnimationLeft;
			isDead = true;

		}
		default:
			break;
		}
	}
	
	if (otherData == DATA_GROUND && !isDead)
	{
		canJump = true;
		canDoubleJump = true;
		switch (lookingAt)
		{
		case RIGHT:
		{
			//currentAnimation = &idleAnimationRight;
			currentAnimation = &landAnimationRight;
			if (onTheAir) countLanding = true;

		}break;
		case LEFT:
		{
			//currentAnimation = &idleAnimationLeft;

			currentAnimation = &landAnimationLeft;
			if (onTheAir) countLanding = true;

		}
		default:
			break;
		}
	}
}

void Player::TakeDamage(int damage)
{
	if (godMode) return;
	health -= damage;
	if (health < 0) isDead = true;
}
void Player::Heal(int heal)
{
	if (godMode) return;
	health += heal;
	if (health > maxHealth) health = maxHealth;
}
void Player::Die()
{
	app->ChangeScene(SCENE_DEATH);
}