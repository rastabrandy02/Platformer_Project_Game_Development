#pragma once
#ifndef __FIREBALL_H__
#define __FIREBALL_H__

#include "Physics.h"
#include "Module.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"
#include "Player.h"

class PhysBody;
struct b2MassData;

class Fireball : public Module, public b2ContactListener
{
public:
	Fireball();
	Fireball(int x, int y, int lookingAt, int id);
	virtual ~Fireball();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool LoadState(pugi::xml_node&) override;
	bool SaveState(pugi::xml_node&) override;

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;
	void Die();

	p2Point <int> position;
	

private:

	
	int id;
	int size = 30;
	bool setToDestroy = false;
	bool isAlive = true;

	float speed = 6.0f;

	int direction;
	int deathTimer = 0;

	PhysBody* fireball;
	b2Fixture* fireballSensor;
	SDL_Rect rec;



	SDL_Texture* fireBallTex;
	Animation* currentAnimation;

	Animation idleAnimationRight;
	Animation idleAnimationLeft;

	Animation dieAnimationRight;
	Animation dieAnimationLeft;
};

#endif 
