#include "AnimatedSprite.hpp"
#include <yaml-cpp/yaml.h>
class Character {
public:
    Character(AnimatedSprite* sprite)
        : sprite(sprite), x(0), y(0), speed(2) {}


    void handleInput(const int code) {
        int dx = 0, dy = 0;
        switch (code) {
            case SDLK_d:
            sprite->setAction("walk_right"); dx = speed;
            break;
            case SDLK_w:
            sprite->setAction("walk_up"); dy = -speed;
            break;
            case SDLK_a:
            sprite->setAction("walk_left"); dx = -speed;
            break;
        }

        x += dx;
        y += dy;
    }

    void update(Uint32 now) {
        sprite->update(now);
    }

    void draw() {
        sprite->draw(x, y);
    }


    static Character* createFromYaml(SDL_Renderer *renderer, const std::string& yamlPath) {
        YAML::Node config = YAML::LoadFile(yamlPath);
        auto spriteNode = config["sprite"];
        if (!spriteNode) throw std::runtime_error("Missing 'sprite' section");

        std::string image = spriteNode["image"].as<std::string>();
        int frameW = spriteNode["frameWidth"].as<int>();
        int frameH = spriteNode["frameHeight"].as<int>();
        int frameTime = spriteNode["frameTime"].as<int>();

        AnimatedSprite* sprite = new AnimatedSprite(renderer, image, frameW, frameH);
        sprite->setFrameTime(frameTime);

        YAML::Node anims = config["animations"];
        if (!anims) throw std::runtime_error("Missing 'animations' section");

        for (auto it = anims.begin(); it != anims.end(); ++it) {
            std::string name = it->first.as<std::string>();
            int row = it->second["row"].as<int>();
            int frames = it->second["frames"].as<int>();
            sprite->addAction(name, row, frames);
        }

        if (anims.begin() != anims.end())
            sprite->setAction(anims.begin()->first.as<std::string>());

        return new Character(sprite);
    }

private:
    AnimatedSprite* sprite;
    int x, y, speed;
};
