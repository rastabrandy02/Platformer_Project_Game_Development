#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Module.h"
#include "App.h"
#include "p2Point.h"

class PhysBody;

struct Object
{
	SDL_Texture* graphic;
	PhysBody* body;
	uint fx;

	Object() : graphic(NULL), body(NULL)
	{}
};

class Player : public Module
{
public:
	Player();
	virtual ~Player();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


	p2Point <float> position;
private:
	SDL_Texture* wizard;

	
	
	SDL_Rect playerRec;
	SDL_Rect* r;
	float speedX = 0.3f;
	int speedY = 20;
	bool isFalling;
	bool isJumping = false;
	int jumpStartPos;
	int maxJump = 150;
	
};

#endif // !__PLAYER_H__