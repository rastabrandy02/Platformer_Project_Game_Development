#pragma once
#ifndef __FLYING_ENEMY_H__
#define __FLYING_ENEMY_H__

#include "Physics.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Entity.h"

class PathFinding;
class PhysBody;
struct b2MassData;

class FlyingEnemy : public Entity, public b2ContactListener
{
public:
	FlyingEnemy();
	FlyingEnemy(int x, int y);
	virtual ~FlyingEnemy();

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

	void Move();
	void Path();
	bool CheckAggro();
	bool aggro;
	int aggroDistance = 10;

	bool setToDestroy = false;
	bool isAlive = true;
	int health = 10;
	SDL_Texture* navigationPath;
	int size = 40;
	b2Vec2 speed = { 2.0f, -2.0f };

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



	SDL_Texture* flyingEnemy;
	Animation* currentAnimation;

	Animation idleAnimationRight;
	Animation idleAnimationLeft;

	Animation flyAnimationRight;
	Animation flyAnimationLeft;

	Animation dieAnimationRight;
	Animation dieAnimationLeft;

};
#endif
