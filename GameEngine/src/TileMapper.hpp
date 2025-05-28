#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "helpers/SDL_RectMath.hpp"
#include <string>
#include <list>
#include <unordered_map>
#include <stdexcept>
#include "Renderer.hpp"
#include "Tilesheet.hpp"
#include <yaml-cpp/yaml.h>
#include <map>
class TileMapper {
private:


public:
    // For each tile, it's position on the map
    struct TilemapDataItem {
        int tile_id;
        std::list<SDL_Point> positions; // Maps tile IDs to their positions
    };
    // returns a tilemaped texture of layerDim dimensions based on tilemapdata struct provided
    static SDL_Texture*  renderTilemap(SDL_Point layerDim, Tilesheet& tilesheet, std::list<TilemapDataItem> tilemapData){
        
        auto layer = Renderer::getEmptyTexture(layerDim);
        auto r = Renderer::getSDLRenderer();
        SDL_Texture* tilesheetTex = Renderer::getTextureFromImage(tilesheet.getTexturePath());
        // Save current render target
        SDL_Texture* prevTarget = SDL_GetRenderTarget(r);

        // Set our texture as the render target
        SDL_SetRenderTarget(r, layer);

        // Optionally clear the texture
        SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
        SDL_RenderClear(r);

        
        for (const auto& tileData : tilemapData) {
            int id = tileData.tile_id;
            for (const auto& pos : tileData.positions) {
                SDL_Point point = pos;

                const Tilesheet::Tile* tile = tilesheet.getTypeInfo(id);
                if (tile) {
                    SDL_Rect srcRect = { tile->frame.x * tile->frame.w, tile->frame.y * tile->frame.h, tile->frame.w, tile->frame.h }; // Get the source rectangle from the tilesheet
                    SDL_Rect destRect = { point.x * tile->frame.w, point.y * tile->frame.h, tile->frame.w, tile->frame.h };// Convert to global coordinates
                    SDL_RenderCopy(r, tilesheetTex, &srcRect, &destRect);
                }
            }
        }
        SDL_DestroyTexture(tilesheetTex);
        // Restore previous render target
        SDL_SetRenderTarget(r, prevTarget);

        return layer;
    }


    // Load tilemap data from a YAML file
    static std::list<TilemapDataItem> loadTilemapData(std::string yamlPath) {
        YAML::Node node = YAML::LoadFile(yamlPath);
        std::list<TilemapDataItem> tilemapDataList;

        for (const auto& tile : node["tilemap"]["tiles"]) {
            TilemapDataItem tilemapDataItem;
            tilemapDataItem.tile_id = tile["tile_id"].as<int>();
            for (const auto& pos : tile["positions"]) {
                int x = pos["x"].as<int>();
                int y = pos["y"].as<int>();
                tilemapDataItem.positions.push_back({x, y});
            }
            tilemapDataList.push_back(tilemapDataItem);
        }
        return tilemapDataList;
    }

};