#pragma once
#include <SDL2/SDL.h>
#include <list>
#include <unordered_map>
#include "../IRenderable.hpp"
#include "../Renderer.hpp"
#include "Tilemap.hpp"
#include "../helpers/SDL_RectMath.hpp"

class TileGroup : public IRenderable {
public:
    struct TileInfo {
        SDL_Rect tile;
        SDL_Rect hitbox;
    };

    TileGroup(const Tilemap& map)
    : tileMap(map)
    {
        texture = Renderer::getTextureFromImage(tileMap.getTexturePath());
    }

    ~TileGroup() {
        if (texture) SDL_DestroyTexture(texture);
    }

    void addTile(const std::string& type, SDL_Point position) {
        const auto* typeInfo = tileMap.getTypeInfo(type);
        if (!typeInfo) return;

        SDL_Rect tile = { position.x, position.y, typeInfo->frame.w, typeInfo->frame.h };
        SDL_Rect hitbox = {
            position.x + typeInfo->hitbox.x,
            position.y + typeInfo->hitbox.y,
            typeInfo->hitbox.w,
            typeInfo->hitbox.h
        };

        tiles[type].push_back({tile, hitbox});
    }

    TileInfo * getTile(const std::string type, int index=0){
        if (!tiles.contains(type)) return NULL;
        return &tiles[type][index];
    }

    bool collidesWith(const SDL_Rect& other, const std::string& type) {
        auto it = tiles.find(type);
        if (it == tiles.end()) return false;

        for (const auto& tileInfo : it->second) {
            if (intersects(tileInfo.hitbox, other)) {
                SDL_LogDebug(0, "Collision");
                return true;
            }
        }
        return false;
    }

bool collidesWith(const std::string& type1, const std::string& type2, const SDL_Point& delta) {
    auto it1 = tiles.find(type1);
    auto it2 = tiles.find(type2);

    if (it1 == tiles.end() || it2 == tiles.end()) 
        return false;

    for (const auto& tile : it1->second) {
        SDL_Rect movedHitbox = tile.hitbox;
        movedHitbox.x += delta.x;
        movedHitbox.y += delta.y;

        for (const auto& other : it2->second) {
            if (intersects(movedHitbox, other.hitbox)) {
                SDL_LogDebug(0, "Collision detected between %s and %s", type1.c_str(), type2.c_str());
                return true;
            }
        }
    }
    return false;
}

    void draw(SDL_Renderer* renderer, SDL_Point = {0, 0}) {
        for (const auto& [type, instances] : tiles) {
            const auto* typeInfo = tileMap.getTypeInfo(type);
            if (!typeInfo) continue;

            for (const auto& tile : instances)
                SDL_RenderCopy(renderer, texture, &typeInfo->frame, &tile.tile);
        }
    }

private:
    const Tilemap& tileMap;
    SDL_Texture* texture;
    std::unordered_map<std::string, std::vector<TileInfo>> tiles;
};
