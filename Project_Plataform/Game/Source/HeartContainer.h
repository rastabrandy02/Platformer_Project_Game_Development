#pragma once
//#ifndef __HEARTCONTAINER_H__
//#define __HEARTCONTAINER_H__

#include "Physics.h"
#include "Entity.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"


class PhysBody;

class HeartContainer : public Entity, public b2ContactListener
{
public:
	HeartContainer(int x, int y);
	~HeartContainer();

	
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool LoadState(pugi::xml_node&) override;
	bool SaveState(pugi::xml_node&) override;

	void Die();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Destroy();
private:
	bool setToDestroy = false;
	bool isAlive = true;
	
	p2Point <int> position;
	PhysBody* heartPb;
	b2Fixture* heartSensor;

	SDL_Texture* heartTexture;

	Animation* currentAnimation;

	Animation heartAnimation;
};

//#endif 
