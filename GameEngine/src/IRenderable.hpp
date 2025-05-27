#pragma once
#include <SDL2/SDL.h>

class IRenderable {
public:
    // To be Defined
    virtual void draw(SDL_Renderer* , SDL_Point = {0,0}) = 0;
};