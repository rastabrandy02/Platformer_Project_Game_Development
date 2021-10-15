#include "Player.h"
#include "App.h"
#include "Defs.h"
#include "Log.h"

Player::Player() :  Module()
{
}

Player::~Player()
{}

// Load assets
bool Player::Awake(pugi::xml_node &config)
{
	LOG("Loading player");
	
	position.x = 0;
	position.y = SCREEN_HEIGHT - 150;
	playerRec = { position.x, position.y, 50, 50 };
	return true;
}
bool Player::Start()
{
	bool ret = true;
	wizard = app->tex->Load("Assets/sprites/mago01.png");
	if (wizard == NULL) LOG("----------------NO CARGA----------");
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
bool Player::Update()
{
	bool ret = true;
	app->render->DrawTexture(wizard, 380, 110);
	/*playerRec.x = position.x;
	playerRec.y = position.y;*/
	/*SDL_SetRenderDrawColor(app->render->renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(app->render->renderer, &playerRec);*/
	
	return ret;
}
bool Player::PostUpdate()
{
	bool ret = true;
	
	
	//SDL_SetRenderDrawColor(app->render->renderer, 255, 0, 0, 255);
	//SDL_RenderDrawRect(app->render->renderer, &playerRec);
	//app->render->DrawRectangle(playerRec, 255, 0, 0, 255);
	//app->render->DrawCircle(0, 0, 50, 255, 0, 0, 255);
	//SDL_SetRenderDrawColor(app->render->renderer, 0, 255, 0, 255);
	
		
	return ret;
}



