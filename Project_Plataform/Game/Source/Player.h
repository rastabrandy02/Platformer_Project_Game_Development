#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Module.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"


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

	bool LoadState(pugi::xml_node&) override;
	bool SaveState(pugi::xml_node&) override;

	p2Point <int> position;
private:
	SDL_Texture* wizard;

	
	
	SDL_Rect playerRec;
	
	int speedX = 1;
	int speedY = 10;
	bool isFalling;
	bool isJumping = false;
	int jumpStartPos;
	int maxJump = 100;
	
	Animation* currentAnimation = nullptr;
	Animation idleAnimation;
	Animation runAnimationRight;
	Animation runAnimationLeft;
	Animation jumpAnimation;
};

#endif // !__PLAYER_H__