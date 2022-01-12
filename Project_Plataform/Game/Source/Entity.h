#pragma once
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "App.h"
#include "Module.h"
#include "p2Point.h"

class Entity : public Module
{
	public:
	Entity(){}
	~Entity(){}

	/*virtual bool Awake(pugi::xml_node& config)
	{
		return true;
	}
	virtual bool Start()
	{
		return true;
	}
	virtual bool PreUpdate()
	{
		return true;
	}
	virtual bool Update(float dt)
	{
		return true;
	}
	virtual bool PostUpdate()
	{
		return true;
	}
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&) override
	{
		return true;
	}
	virtual bool SaveState(pugi::xml_node&) override
	{
		return true;
	}*/

	//void BeginContact(b2Contact* contact);
	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB){}
	virtual void Die(){}

	p2Point <int> position;
};

#endif