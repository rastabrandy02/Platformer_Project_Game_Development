#pragma once
#ifndef __WALKING_ENEMY_H__
#define __WALKING_ENEMY_H__

#include "Physics.h"
#include "Module.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"
#include "Pathfinding.h"

class PathFinding;
class PhysBody;
struct b2MassData;

class WalkingEnemy : public Module, public b2ContactListener
{
public:
	WalkingEnemy();
	virtual ~WalkingEnemy();

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
	const DynArray<iPoint>* currentPath;

private:

	void Walk();

	bool setToDestroy = false;
	bool isAlive = true;
	int health = 10;
	SDL_Rect playerRec;
	SDL_Texture* navigationPath;
	int size = 60;
	b2Vec2 speed = { 2.0f, -5.0f };

	bool canJump = false;
	bool onTheAir = false;

	int lookingAt;
	bool countLanding = false;
	int landingTimer = 0;

	bool isDead = false;
	int deathTimer = 0;

	PathFinding* pathfinding;

	PhysBody* enemy;
	b2Fixture* enemySensor;
	SDL_Rect enemyRec;



	SDL_Texture* walkingEnemy;
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

#endif 
