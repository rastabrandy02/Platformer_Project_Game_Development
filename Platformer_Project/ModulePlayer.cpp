#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
	position.x = 0;
	position.y = SCREEN_HEIGHT - 150;
	playerRec = { position.x, position.y, 50, 50 };
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::PreUpdate()
{
	//For the moment, the gravity (bool isFalling) is applyed depending on the Y position with respect to the white ground, this should be changed in the future
	isFalling = false;
	
	if (position.y < (App->scene_intro->ground.y - playerRec.h)) isFalling = true;
	else if (position.y > App->scene_intro->ground.y - playerRec.h) position.y = App->scene_intro->ground.y - playerRec.h;
	
	if (isFalling)
	{
		position.y+= 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speedX;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speedX;
	}
	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
	{
		
		isJumping = true;
		jumpStartPos = position.y;
	}
	
	if(isJumping) position.y -= speedY;
	if (position.y < (jumpStartPos - maxJump)) isJumping = false;

	return UPDATE_CONTINUE;
}
update_status ModulePlayer::Update()
{
	
	playerRec.x = position.x;
	playerRec.y = position.y;
	SDL_SetRenderDrawColor(App->renderer->renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(App->renderer->renderer, &playerRec);
	return UPDATE_CONTINUE;
}
update_status ModulePlayer::PostUpdate()
{
	
	return UPDATE_CONTINUE;
}



