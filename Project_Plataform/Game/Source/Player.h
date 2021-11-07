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
enum directionOfView
{
	RIGHT = 0,
	LEFT
};
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
	void Die();

	p2Point <int> position;
	
private:
	

	PhysBody* player;
	//PhysBody* playerSensor;
	b2Fixture* playerSensor;
	b2MassData* playerMass;
	
	
	SDL_Rect playerRec;
	
	
	b2Vec2 speed = { 5.0f, -5.0f };
	
	bool godMode = false;
	
	bool canJump = false;
	bool canDoubleJump = false;
	bool onTheAir = false;

	int lookingAt;
	bool countLanding = false;
	int landingTimer = 0;
	
	bool isDead = false;
	int deathTimer = 0;
	

	SDL_Texture* wizard;
	Animation* currentAnimation;

	Animation idleAnimationRight;
	Animation idleAnimationLeft;

	Animation runAnimationRight;
	Animation runAnimationLeft;

	Animation jumpAnimationRight;
	Animation jumpAnimationLeft;

	Animation landAnimationRight;
	Animation landAnimationLeft;

	Animation dieAnimationRight;
	Animation dieAnimationLeft;
};

#endif // !__PLAYER_H__