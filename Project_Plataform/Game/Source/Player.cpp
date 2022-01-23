#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"

Player::Player() :  Entity()
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
	
	
	jumpAnimationRight.PushBack({ 100, 100, 85, 95 });
	jumpAnimationRight.speed = 0.09f;
	jumpAnimationRight.loop = true;

	jumpAnimationLeft.PushBack({ 475,100,80,95 });
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

	
	shootingAnimationRight.PushBack({ 9,366,85,85 });
	shootingAnimationRight.PushBack({ 903,366,85,85 });
	shootingAnimationRight.PushBack({ 200,366,85,85 });
	shootingAnimationRight.speed = 0.07f;
	shootingAnimationRight.loop = false;

	shootingAnimationLeft.PushBack({ 323,366,85,85 });
	shootingAnimationLeft.PushBack({ 447,366,85,85 });
	shootingAnimationLeft.PushBack({ 555,366,85,85 });
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
		player->listener = this;
		
	}
	
	
	
	
	return ret;
}

// Unload assets
bool Player::CleanUp()
{
	LOG("Unloading player");
	
	fireballs.Clear();

	return true;
}

// PreUpdate: Imput
bool Player::PreUpdate()
{
	bool ret = true;

	uint fireballCount2 = 0; 

	if (fireballZero == false)
	{
		for (ListItem<Fireball*>* item = fireballs.start; item != NULL; item = item->next)
		{
			item->data->PreUpdate();
		}
	}

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
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && canAttack)
		{
			int offset = 50;
			if (lookingAt == RIGHT)
			{
				currentAnimation = &shootingAnimationRight;
				Fireball* fireball = new Fireball(METERS_TO_PIXELS(player->body->GetPosition().x) + offset, METERS_TO_PIXELS(player->body->GetPosition().y), lookingAt, fireID);
				fireballs.Add(fireball);
				fireball->Start();
				fireID++;
			}
			if (lookingAt == LEFT)
			{
				currentAnimation = &shootingAnimationLeft;
				Fireball* fireball = new Fireball(METERS_TO_PIXELS(player->body->GetPosition().x) - offset, METERS_TO_PIXELS(player->body->GetPosition().y), lookingAt, fireID);
				fireballs.Add(fireball);
				fireball->Start();
				fireID++;
				
			}
	
				
			/*for (ListItem<Fireball*>* item = fireballs.start; item; item = item->next)
			{
					item->data->Start();
			}*/
				canAttack = false;
		}
		

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
		if (canAttack == false)
		{
			fireballTimer++;
			if (fireballTimer > 30)
			{
				canAttack = true;
				fireballTimer = 0;
			}
		}

		currentAnimation->Update();
		if (fireballZero == false)
		{
			for (ListItem<Fireball*>* item = fireballs.start; item != NULL; item = item->next)
			{

				item->data->Update(dt);
			}
		}

		
	}
	
	return ret;
}
bool Player::PostUpdate()
{
	bool ret = true;
	if (app->currentScene == SCENE_LEVEL_1)
	{
		SDL_Rect section = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(wizard, METERS_TO_PIXELS(player->body->GetPosition().x) - 35, METERS_TO_PIXELS(player->body->GetPosition().y) - 50, &section);
		if (stars > 3) stars = 3;
		

		int playerPosX;
		int playerPosY;
		player->GetPosition(playerPosX, playerPosY);

		if (playerPosX >= app->render->camera.x + 300)
		{
			app->render->camera.x = -playerPosX + 300;
			app->scene->UpdateHealthBar(playerPosX -290, 10);
		}

		if (fireballZero == false)
		{
			for (ListItem<Fireball*>* item = fireballs.start; item; item = item->next)
			{

				item->data->PostUpdate();
			}
		}
		
	}
	
		
	return ret;
}

bool Player::LoadState(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();
	player->body->SetTransform({ (float)position.x,(float)position.y }, 0);

	health = node.child("health").attribute("value").as_int();
	stars = node.child("stars").attribute("value").as_int();


	int playerPosX;
	int playerPosY;
	player->GetPosition(playerPosX, playerPosY);
	if (playerPosX >= app->render->camera.x + 300)
	{
		app->render->camera.x = -playerPosX + 300;
		app->scene->UpdateHealthBar(playerPosX - 290, 10);
	}
	else app->scene->UpdateHealthBar(app->render->camera.x, 10);
	return true;
}
bool Player::SaveState(pugi::xml_node& node)
{
	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x").set_value(player->body->GetPosition().x);
	pos.append_attribute("y").set_value(player->body->GetPosition().y +1);

	pugi::xml_node health = node.append_child("health");
	health.append_attribute("value").set_value(health);

	pugi::xml_node stars = node.append_child("stars");
	stars.append_attribute("value").set_value(stars);
	
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
void Player::CollectStar()
{
	stars++;
}
void Player::Die()
{
	app->ChangeScene(SCENE_DEATH);
}
int Player::GetHealth()
{
	return health;
}
int Player::GetStars()
{
	return stars;
}