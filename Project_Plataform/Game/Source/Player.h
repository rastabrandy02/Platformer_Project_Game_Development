#pragma once
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
	bool Update();
	bool PostUpdate();
	bool CleanUp();

public:
	SDL_Texture* wizard;

	p2Point <int> position;
	
	SDL_Rect playerRec;
	SDL_Rect* r;
	int speedX = 10;
	int speedY = 20;
	bool isFalling;
	bool isJumping = false;
	int jumpStartPos;
	int maxJump = 150;
	
};