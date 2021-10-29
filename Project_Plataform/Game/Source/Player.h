#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Physics.h"
#include "Module.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"


class PhysBody;
struct b2MassData;

struct Object
{
	SDL_Texture* graphic;
	PhysBody* body;
	uint fx;

	Object() : graphic(NULL), body(NULL)
	{}
};

class Player : public Module, public b2ContactListener
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


	void BeginContact(b2Contact* contact);

	p2Point <int> position;
	
private:
	

	PhysBody* player;
	//PhysBody* playerSensor;
	b2Fixture* playerSensor;
	b2MassData* playerMass;
	
	
	SDL_Rect playerRec;
	
	
	b2Vec2 speed = { 1.0f, -3.0f };
	
	bool isFalling;
	
	int jumpStartPos;
	int maxJump = 100;
	bool canJump = false;
	bool canDoubleJump = false;
	
	

	SDL_Texture* wizard;
	Animation* currentAnimation;
	Animation idleAnimation;
	Animation runAnimationRight;
	Animation runAnimationLeft;
	Animation jumpAnimation;
};

#endif // !__PLAYER_H__