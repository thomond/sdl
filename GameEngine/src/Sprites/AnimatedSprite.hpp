#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "../IRenderable.hpp"
#include "../Renderer.hpp"
class AnimatedSprite  :IRenderable {
public:
    AnimatedSprite(const std::string& path,
                   int frameWidth, int frameHeight)
        : frameWidth(frameWidth), frameHeight(frameHeight),
          currentFrame(0), frameTime(100), lastUpdate(0), playing(true)
    {
        texture = Renderer::getTextureFromImage(path);
    }

    ~AnimatedSprite() {
        if (texture) SDL_DestroyTexture(texture);
    }

    void addAction(const std::string& name, int row, int frameCount) {
        actions[name] = { row, frameCount };
    }

    void setAction(const std::string& name) {
        if (name != currentAction && actions.count(name)) {
            currentAction = name;
            currentFrame = 0;
            lastUpdate = SDL_GetTicks();
        }
    }

    std::string getCurrentAction(){
        return currentAction;
    }

    int getCurrentFrame(){
        return currentFrame;
    }

    void update(Uint32 now) {
        if (!playing || currentAction.empty()) return;

        if (now - lastUpdate >= frameTime) {
            currentFrame = (currentFrame + 1) % actions[currentAction].frameCount;
            lastUpdate = now;
        }
    }

    void draw(SDL_Renderer* renderer, SDL_Point loc) {
        if (currentAction.empty()) return;

        ActionInfo& action = actions[currentAction];
        SDL_Rect src = {
            currentFrame * frameWidth,
            action.row * frameHeight,
            frameWidth,
            frameHeight
        };
        SDL_Rect dst = { loc.x, loc.y, frameWidth, frameHeight };
        SDL_RenderCopy(renderer, texture, &src, &dst);
    }

    void setFrameTime(Uint32 ms) { frameTime = ms; }
    void setPlaying(bool play) { playing = play; }

private:
    struct ActionInfo {
        int row;
        int frameCount;
    };

    SDL_Texture* texture;
    int frameWidth, frameHeight;
    int currentFrame;
    Uint32 frameTime;
    Uint32 lastUpdate;

    bool playing;

    std::unordered_map<std::string, ActionInfo> actions;
    std::string currentAction;
};