#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

    GuiSlider(uint32 id, SDL_Rect bounds, const char* text);
    virtual ~GuiSlider();

    bool Update(Input* input, float dt);
    bool DrawDebug(Render* render);
    bool DrawTexture(Render* render);
public:

    // GuiSlider specific properties
    // Maybe some animation properties for state change?
    SDL_Rect slider;
public:
    int value;

    int minValue;
    int maxValue;
};

#endif // __GUISLIDER_H__
