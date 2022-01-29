#ifndef __SCENES_H__
#define __SCENES_H__

#include "SString.h"

class Input;
class Render;
class Textures;

class GuiControl;

enum class SceneType
{
    LOGO,
    TITLE,
    GAMEPLAY,
    LOAD_GAMEPLAY,
    SETTINGS,
    WIN,
    DEATH,
    PAUSE,
    CREDITS
};

class SCENES
{
public:

    SCENES() : active(true), loaded(false), transitionRequired(false) {}

    virtual bool Load(Textures* tex)
    {
        return true;
    }

    virtual bool Update(Input* input, float dt)
    {
        return true;
    }

    virtual bool Draw(Render* render)
    {
        return true;
    }

    virtual bool Unload()
    {
        return true;
    }

    void TransitionToScene(SceneType scene)
    {
        transitionRequired = true;
        nextScene = scene;
    }

    virtual bool OnGuiMouseClickEvent(GuiControl* control)
    {
        return true;
    }

public:

    bool active = true;
    SString name;  

    bool loaded = false;

    bool transitionRequired;
    SceneType nextScene;
    SceneType currentScene;
    SceneType previousScene;
};

#endif // __SCENES_H__