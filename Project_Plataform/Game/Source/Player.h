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
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

public:

	Object ball;
	Object flipper1;
	Object flipper2;
	Object spring;

	PhysBody* flipper1_wheel;
	PhysBody* flipper2_wheel;
	PhysBody* spring_wheel;

	Object flipper_up1;
	Object flipper_up2;
	PhysBody* flipper_up1_wheel;
	PhysBody* flipper_up2_wheel;

	PhysBody* sensor1;

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