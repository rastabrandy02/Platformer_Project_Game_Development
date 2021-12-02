#pragma once
//#ifndef __HEARTCONTAINER_H__
//#define __HEARTCONTAINER_H__

#include "Physics.h"
#include "Module.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"


class PhysBody;

class HeartContainer : public Module, public b2ContactListener
{
public:
	HeartContainer();
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
private:
	p2Point <int> position;
	PhysBody* heartPb;
	b2Fixture* heartSensor;

	SDL_Texture* heartTexture;
	Animation* currentAnimation;

	Animation normalAnimation;
};

//#endif 
