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


	name.Create("player");
}

Player::~Player()
{}

// Load assets
bool Player::Awake(pugi::xml_node &config)
{
	LOG("Loading player");
	
	position.x = 0;
	position.y = 250;
	
	return true;
}
bool Player::Start()
{
	bool ret = true;
	
	//wizard = app->tex->Load("Assets/sprites/wizard_spritesheet.png");
	wizard = app->tex->Load("Assets/sprites/wizard_spritesheet_extended.png");
	
	player = app->physics->CreateCircle(position.x, position.y, 25);
	
	player->body->SetFixedRotation(true);
	player->body->GetFixtureList()->SetFriction(5.0f);
	
	b2PolygonShape sensorShape;
	sensorShape.SetAsBox(PIXEL_TO_METERS(26),PIXEL_TO_METERS (26));
	
	
	b2FixtureDef sensorFixture;
	sensorFixture.shape = &sensorShape;
	sensorFixture.isSensor = true;
	
	playerSensor = player->body->CreateFixture(&sensorFixture);
	playerSensor->SetUserData((void*)1);
	
	
	
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

	
	
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		
		player->body->SetLinearVelocity({ speed.x, player->body->GetLinearVelocity().y });

		lookingAt = RIGHT;
		if(!onTheAir)currentAnimation = &runAnimationRight;
		if (onTheAir && !countLanding) currentAnimation = &jumpAnimationRight;
		
	}
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		
		
		player->body->SetLinearVelocity({ -speed.x, player->body->GetLinearVelocity().y });
		//playerVelocity = player->body->GetLinearVelocity();
		//position.x = player->body->GetPosition().x;
		lookingAt = LEFT;
		if(!onTheAir)currentAnimation = &runAnimationLeft;
		if (onTheAir && !countLanding) currentAnimation = &jumpAnimationLeft;
	}
	if (canJump || canDoubleJump)
	{
		if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
		{
			


			player->body->SetLinearVelocity({ player->body->GetLinearVelocity().x , speed.y});
			onTheAir = true;
			//playerVelocity = player->body->GetLinearVelocity();
			
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
	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE) && (app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) && (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) && !onTheAir)
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
		//currentAnimation = &idleAnimationRight;
	
	
	
	return true;
}
bool Player::Update(float dt)
{
	bool ret = true;
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
	
	currentAnimation->Update();
	return ret;
}
bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect section = currentAnimation->GetCurrentFrame();
	//app->render->DrawTexture(wizard, position.x,position.y, &section);
	app->render->DrawTexture(wizard, METERS_TO_PIXELS(player->body->GetPosition().x) -35,METERS_TO_PIXELS( player->body->GetPosition().y)-50, &section);
	
	
	
		
	return ret;
}
void Player::BeginContact(b2Contact* contact)
{
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	
	if (((int)fixtureUserData == 1))
	{
		canJump = true;
		canDoubleJump = true;
		switch (lookingAt)
		{
		case RIGHT:
		{
			//currentAnimation = &idleAnimationRight;
				currentAnimation = &landAnimationRight;
				if(onTheAir) countLanding = true;
			
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
	
	 fixtureUserData = contact->GetFixtureB()->GetUserData();
	if (((int)fixtureUserData == 1))
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


	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);

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
