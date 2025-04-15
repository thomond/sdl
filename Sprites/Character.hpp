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
            case SDLK_s:
            sprite->setAction("walk_down"); dy = speed;
            break;
        }

        x += dx;
        y += dy;
    }

    void update(Uint32 now) {
        sprite->update(now);
        currentBoundingBox = getBoundingBoxFrame(sprite->getCurrentAction(), sprite->getCurrentFrame());
    }

    void draw() {
        sprite->draw(x, y);
        SDL_SetRenderDrawColor(sprite->getRenderer(), 255, 255, 255, 128); // Set render to white to display collision rects
        SDL_RenderDrawRect(sprite->getRenderer(), new SDL_Rect({ currentBoundingBox.x+x ,currentBoundingBox.y+y ,currentBoundingBox.w, currentBoundingBox.h}) );
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
        Character * ch = new Character(sprite);

        sprite->setFrameTime(frameTime);

        YAML::Node anims = spriteNode["animations"];
        if (!anims) throw std::runtime_error("Missing 'animations' section");

        for (auto it = anims.begin(); it != anims.end(); ++it) {
            std::string name = it->first.as<std::string>();
            int row = it->second["row"].as<int>();
            int frames = it->second["frames"].as<int>();
            sprite->addAction(name, row, frames);
        

            YAML::Node rects = it->second["BoundingBoxs"];
            if (rects && rects.IsSequence()) {
                for (size_t frame = 0; frame < rects.size(); ++frame) {
                    auto rect = rects[frame];
                    SDL_Rect r = {
                        rect["cx"].as<int>(),
                        rect["cy"].as<int>(),
                        rect["cw"].as<int>(),
                        rect["ch"].as<int>()
                    };
                    ch->addBoundingBox(name, frame, r); // You must implement this method
                }
            }
        }

        if (anims.begin() != anims.end())
            sprite->setAction(anims.begin()->first.as<std::string>());

        return ch;
    }

    void addBoundingBox(std::string name, int frame, SDL_Rect r){
        BoundingBoxes[name][frame] = r;
    }

    SDL_Rect getBoundingBoxFrame(std::string name, int frame){
        return BoundingBoxes[name][frame];
    }

private:
    AnimatedSprite* sprite;
    int x, y, speed;

    // Ordered map for Ani actions pointing to another map 
    std::unordered_map<std::string, std::unordered_map<int,SDL_Rect>> BoundingBoxes;
    SDL_Rect currentBoundingBox;
};
