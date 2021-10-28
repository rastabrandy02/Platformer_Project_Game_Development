#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"

Player::Player() :  Module()
{
	idleAnimation.PushBack({ 10, 8, 85, 95 });
	currentAnimation = &idleAnimation;

	runAnimationRight.PushBack({ 10, 8, 85, 95 });
	runAnimationRight.PushBack({ 85,8,85,95 });
	runAnimationRight.PushBack({ 167,8,85,95 });
	runAnimationRight.PushBack({ 248,8,85,95 });
	runAnimationRight.speed = 0.003f;
	runAnimationRight.loop = true;

	runAnimationLeft.PushBack({ 330,8,85,95 });
	runAnimationLeft.PushBack({ 405,8,85,95 });
	runAnimationLeft.PushBack({ 485,8,85,95 });
	runAnimationLeft.PushBack({ 566,8,85,95 });
	runAnimationLeft.speed = 0.003f;
	runAnimationLeft.loop = true;
	
	
	jumpAnimation.PushBack({ 10, 100, 85, 95 });
	jumpAnimation.PushBack({ 100, 100, 85, 95 });
	jumpAnimation.speed = 0.003f;
	jumpAnimation.loop = true;
}

Player::~Player()
{}

// Load assets
bool Player::Awake(pugi::xml_node &config)
{
	LOG("Loading player");
	
	position.x = 0;
	position.y = 250;
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
		currentAnimation = &runAnimationRight;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speedX;
		currentAnimation = &runAnimationLeft;
	}
	
	if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
	{
		
		isJumping = true;
		currentAnimation = &jumpAnimation;
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
	app->render->DrawTexture(wizard, position.x,position.y, &section);
	
	
	
		
	return ret;
}

bool Player::LoadState(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();
	return true;
}
bool Player::SaveState(pugi::xml_node& node)
{
	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x").set_value(position.x);
	pos.append_attribute("y").set_value(position.y);
	
	return true;
}
