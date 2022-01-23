#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Physics.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&) override;
	bool SaveState(pugi::xml_node&) override;

	void UpdateHealthBar(int x, int y);

private:
	SDL_Texture* titlleBackground;
	SDL_Texture* deathBackground;
	
	List <Module*> enemies;
	
	
	SDL_Rect healthBar;
	SDL_Rect healthBarBackground;
	SDL_Rect emptyBar;

	SDL_Texture* starTex;
	SDL_Texture* emptyStarTex;
};

#endif // __SCENE_H__