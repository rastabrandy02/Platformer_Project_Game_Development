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
	bool starsLoaded = false;
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		SString name = item->data->name.GetString();

		int tempId = item->data->id;
		if (tempId != -1)
		{
			item->data->LoadState(node.child(name.GetString()));
			item = item->next;
			item->data->LoadState(node.child(name.GetString()).next_sibling());
			item = item->next;
			item->data->LoadState(node.child(name.GetString()).next_sibling().next_sibling());

		}
		//if (name == "star" && !starsLoaded)
		//{
		//	pugi::xml_node starNode = node.child("star");
		//	for (ListItem<Star*>* starItem = stars.start; starItem; starItem = starItem->next)
		//	{
		//		//starItem->data->LoadState(starNode.child(name.GetString()));
		//		starItem->data->LoadState(starNode);
		//		starNode = starNode.next_sibling();
		//	}
		//	starsLoaded = true;
		//}
		//else item->data->LoadState(node.child(name.GetString
		item->data->LoadState(node.child(name.GetString()));
		
	}
	
	return true;
}
bool EntityManager::SaveState(pugi::xml_node& node)
{
	int starsSaved = 0;
	for (ListItem<Entity*>* item = entities.start; item; item = item->next)
	{
		
		SString name = item->data->name.GetString();
		//if (name == "star" && starsSaved < 3)
		//{
		//	pugi::xml_node starNode = node.child("star");
		//	/*for (ListItem<Star*>* starItem = stars.end; starItem; starItem = starItem->next)
		//	{*/
		//		//starItem->data->SaveState(starNode.child(name.GetString()));
		//		if(starsSaved == 0)item->data->SaveState(node.child(name.GetString()));
		//		//starItem->data->SaveState(starNode);
		//		

		//		if (starsSaved == 1)item->data->SaveState(node.child(name.GetString()).next_sibling());
		//		
		//		if (starsSaved == 2)item->data->SaveState(node.child(name.GetString()).next_sibling().next_sibling());
		//		
		//	//}
		//	starsSaved++;
		//}
		//else item->data->SaveState(node.append_child(name.GetString()));
		item->data->SaveState(node.append_child(name.GetString()));
	}


	return true;
}

void EntityManager::CreateEntity(EntityType type, int x, int y, int id)
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
		HeartContainer* item = new HeartContainer(x, y);
		item->Start();
		entities.Add(item);
	}
		break;
	case STAR:
	{
		Star* item = new Star(x, y,id);
		item->Start();
		entities.Add(item);
		stars.Add(item);
	}
		break;
	default:
		break;
	}
	
}