#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <list>
#include <unordered_map>
#include <stdexcept>
#include "IRenderable.hpp"
#include <iostream>
#include "GraphicsObject.hpp"

class Renderer {
public:
    

    static Renderer* init(std::string title="SDL Window", SDL_Rect frame=SDL_Rect{0,0,800,600}, Uint32 winflags=0){
        if (!instance){
            if (SDL_Init(SDL_INIT_VIDEO) != 0) throw std::runtime_error(SDL_GetError());
            if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) throw std::runtime_error(IMG_GetError());
            instance = new Renderer();
            instance->win = SDL_CreateWindow(title.c_str(), frame.x, frame.y, frame.w, frame.h, winflags);
            instance->renderer = SDL_CreateRenderer(instance->win, -1, SDL_RENDERER_ACCELERATED); 
            if (!instance->win || !instance->renderer) throw std::runtime_error(SDL_GetError());
        }
        return instance;
    }

    static void destroy(){
        if(instance){
            SDL_DestroyRenderer(instance->renderer);
            SDL_DestroyWindow(instance->win);
            IMG_Quit();
            SDL_Quit();
            delete instance;
            instance = nullptr;
        }
        
    }
      
    static SDL_Renderer* getSDLRenderer() {
        if (!instance || !instance->renderer)
            throw std::runtime_error("Renderer not initialized");
        return instance->renderer;
    }

    static SDL_Texture* getTextureFromSurface(SDL_Surface* surface){
        auto renderer = getSDLRenderer();

        return SDL_CreateTextureFromSurface(renderer, surface); 
    }

    static SDL_Texture* getTextureFromImage(const std::string& path){
        auto renderer = getSDLRenderer();
        
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) throw std::runtime_error(IMG_GetError());
        
        
        SDL_Texture * texture =  getTextureFromSurface(surface); 
        if(!texture) throw std::runtime_error(SDL_GetError());
        
        SDL_FreeSurface(surface);
        
        return texture;

    }

    static SDL_Texture* getEmptyTexture(SDL_Point dim){
        auto renderer = getSDLRenderer();
        SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dim.x, dim.y);
        if (!texture) throw std::runtime_error(SDL_GetError());
        return texture;
    }

    static void addToRenderList(GraphicsObject * r){
         if(!instance) throw std::runtime_error("Renderer not initialised.");
        renderList.push_back(r);
    }

    static void draw(bool screenshot = false){
        auto renderer = getSDLRenderer();
        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
        SDL_RenderClear(renderer);
        // Draw renderables in list
        for (auto *graphicsObject: renderList)
            SDL_RenderCopy(renderer, graphicsObject->texture, nullptr, &graphicsObject->vector);
        if (screenshot) {
            dump_screen("screenshot.png");
        }
        SDL_RenderPresent(renderer);
    }

    /*static void draw(SDL_Texture * texture=nullptr, SDL_Rect destRect = {0, 0, 0, 0}) {
        auto renderer = getSDLRenderer();
        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
        SDL_RenderClear(renderer);
        // Draw renderables in list
        for (auto *graphicsObject: renderList)
            draw(graphicsObject);
        if (texture) {
            if(SDL_RectEmpty(&destRect)) {
                int w, h;
                SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
                destRect = {0, 0, w, h}; // Use texture size if destRect is empty
            }
            SDL_RenderCopy(renderer, texture, nullptr, &destRect);
        }
        SDL_RenderPresent(renderer);
    }*/


    /*static void draw(GraphicsObject * obj) {
        draw(obj->texture, obj->vector);

    }*/

    // TODO: Test this
    static bool dump_screen(const char* filename) {
        
        int w, h;
        SDL_GetRendererOutputSize(getSDLRenderer(), &w, &h);

        SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
        
        if (!surface) {
            std::cerr << "CreateRGBSurface failed: " << SDL_GetError() << "\n";
            return false;
        }
        SDL_LockSurface(surface);
        if (SDL_RenderReadPixels(getSDLRenderer(), nullptr, surface->format->format,
                                surface->pixels, surface->pitch) != 0) {
            std::cerr << "RenderReadPixels failed: " << SDL_GetError() << "\n";
            SDL_UnlockSurface(surface);
            SDL_FreeSurface(surface);
            return false;
        }

        if (IMG_SavePNG(surface, filename) != 0) {
            std::cerr << "IMG_SavePNG failed: " << IMG_GetError() << "\n";
            SDL_UnlockSurface(surface);
            SDL_FreeSurface(surface);
            return false;
        }
        SDL_UnlockSurface(surface);
        SDL_FreeSurface(surface);
        return true;
    }

    private:
    Renderer() = default;
    ~Renderer() = default;

    static Renderer*        instance;
    static std::list<GraphicsObject *> renderList;
    SDL_Renderer*           renderer = nullptr;
    SDL_Window*             win = nullptr;

    class RenderLayer : public IRenderable{

        private:
        std::string name;
        int index;
        SDL_Texture * texture;
        public:
            RenderLayer(std::string name, int index, SDL_Texture* texture)
                : name(name), index(index), texture(texture) {}

            void draw(SDL_Renderer* renderer) {
                // Draw the texture using the renderer
            }
    };
};