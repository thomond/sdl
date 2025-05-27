#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>
#include <yaml-cpp/yaml.h>

class Tilesheet {
public:
    struct Tile {

        //SDL_Rect hitbox;
        SDL_Rect frame ; // x and y coordinates of the tile in the tilesheet width and height of the tile
        int id;
    };
    // Returns a pointer to a new SDL_Rect with the global coordinates of the tile frame
    SDL_Rect * getFrameGlobal(int id) {

        auto frame = getTypeInfo(id)->frame;
        return new SDL_Rect{frame.x*frame.w, frame.y*frame.h, frame.w, frame.h};
    }

    static Tilesheet createFromYaml(const std::string& yamlPath) {
        YAML::Node node = YAML::LoadFile(yamlPath);
        Tilesheet map;
        map.texturePath = node["tilesheet"]["image"].as<std::string>();
        int frameWidth = node["tilesheet"]["frameWidth"].as<int>();
        int frameHeight = node["tilesheet"]["frameHeight"].as<int>(); 

        for (const auto& tile : node["tilesheet"]["tiles"]) {
            std::string name = tile.first.as<std::string>();
            int row = tile.second["row"].as<int>();
            int col = tile.second["col"].as<int>();
            int id = tile.second["id"].as<int>();

            //SDL_Rect position = { col, row, map.frameWidth, map.frameHeight };
            //SDL_Rect hitbox = { 0, 0, map.frameWidth, map.frameHeight };

            map.tileTypes[name] = Tile{SDL_Rect{col,row,frameWidth,frameHeight}, id};
        }

        return map;
    }

    const Tile* getTypeInfo(const std::string& type) const {
        auto it = tileTypes.find(type);
        if (it != tileTypes.end())
            return &it->second;
        return nullptr;
    }


    const Tile* getTypeInfo(int id) const {
        for (const auto& pair : tileTypes) {
            if (pair.second.id == id)
                return &pair.second;
        }
        return nullptr;
    }

    const std::unordered_map<std::string, Tile>& getTiles() const { return tileTypes; }

    const std::string& getTexturePath() const { return texturePath; }

private:
    std::string texturePath;
    std::unordered_map<std::string, Tile> tileTypes;
};
