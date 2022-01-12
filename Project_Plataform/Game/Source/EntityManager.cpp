#include "EntityManager.h"

EntityManager::EntityManager()
{
	name.Create("entity_manager");
}
EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& config)
{
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		item->data->Awake(config);
	}

	return true;
}
bool EntityManager::Start()
{
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		item->data->Start();
	}

	return true;
}
bool EntityManager::PreUpdate()
{
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		item->data->PreUpdate();
	}

	return true;
}
bool EntityManager::Update(float dt)
{
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		item->data->Update(dt);
	}

	return true;
}
bool EntityManager::PostUpdate()
{
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		item->data->PostUpdate();
	}

	return true;
}
bool EntityManager::CleanUp()
{
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		item->data->CleanUp();
	}
	entities.Clear();
	return true;
}

bool EntityManager::LoadState(pugi::xml_node& node)
{
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		SString name = item->data->name.GetString();
		item->data->LoadState(node.parent().child(name.GetString()));
	}
	
	return true;
}
bool EntityManager::SaveState(pugi::xml_node& node)
{
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		SString name = item->data->name.GetString();
		item->data->SaveState(node.parent().append_child(name.GetString()));
	}


	return true;
}

void EntityManager::CreateEntity(EntityType type, int x, int y)
{
	switch (type)
	{
	case ENTITY:
	{
		Entity* item = new Entity();
		entities.Add(item);
	}
		break;
	case PLAYER:
	{
		Player* item = new Player();
		item->position.x = 100;
		item->position.y = 290;
		item->Start();
		app->player = item;
		entities.Add(item);
		
	}
		break;
	case WALKING_ENEMY:
	{
		WalkingEnemy* item = new WalkingEnemy(x, y);
		item->Start();
		entities.Add(item);
	}
		break;
	case FLYING_ENEMY:
	{
		FlyingEnemy* item = new FlyingEnemy(x, y);
		item->Start();
		entities.Add(item);
	}
		break;
	case HEART:
	{
		HeartContainer* item = new HeartContainer();
		item->Start();
		entities.Add(item);
	}
		break;
	case STAR:
	{
		Star* item = new Star();
		item->Start();
		entities.Add(item);
	}
		break;
	default:
		break;
	}
	
}