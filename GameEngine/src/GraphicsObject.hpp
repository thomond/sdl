#pragma once
#include <SDL2/SDL_render.h>
#include <stdexcept>

struct GraphicsObject {
    SDL_Texture * texture = nullptr;
    SDL_Rect vector = {0, 0, 0, 0};
};