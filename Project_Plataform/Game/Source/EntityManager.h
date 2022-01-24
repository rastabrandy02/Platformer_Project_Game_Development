#pragma once

#include "Module.h"
#include "App.h"
#include "Entity.h"
#include "p2Point.h"

class Entity;
enum EntityType
{
	ENTITY = 0,
	PLAYER,
	WALKING_ENEMY,
	FLYING_ENEMY,
	HEART,
	STAR

};
class EntityManager : public Module
{
public:
	EntityManager();
	virtual ~EntityManager();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool LoadState(pugi::xml_node&) override;
	bool SaveState(pugi::xml_node&) override;

	void CreateEntity(EntityType type, int x, int y, int id = -1);

	p2Point <int> position;

	List <Entity*> entities;
	List <Star*> stars;
};