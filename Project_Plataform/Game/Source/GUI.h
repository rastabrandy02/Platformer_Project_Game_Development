/*
#ifndef __GUI_H__
#define __GUI_H__

#include "Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "DynArray.h"

#define CURSOR_WIDTH 2

#define WHITE {255,255,255}
#define BLUE {0,0,255}
#define DEFAULT_TEXT_SIZE 12

enum class FOCUS
{
	OUT_OF_FOCUS = 0,
	GET_FOCUS,
	ON_FOCUS,//Same as hover
	LOSE_FOCUS,
	MAX
};

class GUIElement
{
public:
	

public:
	GUIElement(const iPoint& position);
	virtual bool PreUpdate();
	virtual bool PostUpdate();
	virtual bool CleanUp();

	void SetFocus(bool focus);
	void SetParent(GUIElement* parent);
	bool CheckBounds(int x, int y);
	void DrawOutline();
	void SetGlobalPos(iPoint pos);
	iPoint GetGlobalPos();
	void SetActive(bool active);
	bool IsActive();
	
};

enum class GUI_ADJUST
{
	
};

// ---------------------------------------------------
class Gui : public Module
{
public:

	Gui();

	// Destructor
	virtual ~Gui();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

	void LoadGUISfx(pugi::xml_node& node);




	//Create GUI Objects
	
	


};




#endif
*/
