#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"

Player::Player() :  Module()
{
	idleAnimation.PushBack({ 30, 16, 90, 120 });
	idleAnimation.PushBack({ 140,16,90,120 });
	idleAnimation.PushBack({ 260,16,90,120 });
	idleAnimation.PushBack({ 380,16,90,120 });
	idleAnimation.speed = 0.0005f;
	idleAnimation.loop = true;
	currentAnimation = &idleAnimation;
	
}

Player::~Player()
{}

// Load assets
bool Player::Awake(pugi::xml_node &config)
{
	LOG("Loading player");
	
	position.x = 0;
	position.y = SCREEN_HEIGHT - 150;
	//playerRec = { position.x, position.y, 50, 50 };
	return true;
}
bool Player::Start()
{
	bool ret = true;
	//wizard = app->tex->Load("Assets/sprites/mago01.png");
	wizard = app->tex->Load("Assets/sprites/wizard_spritesheet.png");
	
	return ret;
}

// Unload assets
bool Player::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
bool Player::PreUpdate()
{
	bool ret = true;
	//For the moment, the gravity (bool isFalling) is applyed depending on the Y position with respect to the  ground, this should be changed in the future
	isFalling = false;
	
	/*if (position.y < (app->scene->ground.y - playerRec.h)) isFalling = true;
	else if (position.y > app->scene->ground.y - playerRec.h) position.y = app->scene->ground.y - playerRec.h;*/
	
	if (isFalling)
	{
		position.y+= 10;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speedX;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speedX;
	}
	
	if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
	{
		
		isJumping = true;
		jumpStartPos = position.y;
	}
	
	if(isJumping) position.y -= speedY;
	if (position.y < (jumpStartPos - maxJump)) isJumping = false;

	return true;
}
bool Player::Update(float dt)
{
	bool ret = true;
	
	
	currentAnimation->Update();
	return ret;
}
bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect section = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(wizard, (int)position.x, (int)position.y, &section);
	
	
	
		
	return ret;
}



