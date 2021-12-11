#pragma once
#include "Physics.h"
#include "Module.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"


class PhysBody;

class Star : public Module, public b2ContactListener
{
public:
	Star();
	~Star();


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
	bool draw = true;
	p2Point <int> position;
	PhysBody* starPb;
	b2Fixture* starSensor;

	SDL_Texture* starTexture;

	Animation* currentAnimation;

	Animation starAnimation;
};

//#endif 
