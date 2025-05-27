#include "AnimatedSprite.hpp"
#include <yaml-cpp/yaml.h>
#include "../IRenderable.hpp"
class Character : public IRenderable {
public:
    Character(AnimatedSprite* sprite)
        : sprite(sprite), x(100), y(100),dx(0),dy(0), speed(2) {}

    void handleInput(const int code) {
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
    }

    bool willIntersect(SDL_Rect * boundingBox){
        SDL_Rect deltaRect = getGlobalBoundingBox() + SDL_Point{dx , dy};// {currentBoundingBox.x+dx ,currentBoundingBox.y+dy ,currentBoundingBox.w, currentBoundingBox.h};
        if(intersects(deltaRect , *boundingBox)){
            return true;
        }else{
            return false;
        }
    }

    void resetDelta(){
        dx = 0, dy = 0;
    }

    void update(Uint32 now) {
        sprite->update(now);
        x += dx;
        y += dy;
        dx = 0, dy = 0;// reset delta
    }

    void draw(SDL_Renderer * renderer, SDL_Point) {
        sprite->draw(renderer, {x,y});
        #if defined DEBUG
        SDL_Rect currentBoundingBox =  getCurrentBoundingBox();
        SDL_Rect debugRect = { currentBoundingBox.x + x, currentBoundingBox.y + y, currentBoundingBox.w, currentBoundingBox.h };

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128); // Set render to white to display collision rects
        SDL_RenderDrawRect(renderer, &debugRect );
        #endif
    }


    static Character* createFromYaml(const std::string& yamlPath) {
        YAML::Node config = YAML::LoadFile(yamlPath);
        auto spriteNode = config["sprite"];
        if (!spriteNode) throw std::runtime_error("Missing 'sprite' section");

        std::string image = spriteNode["image"].as<std::string>();
        int frameW = spriteNode["frameWidth"].as<int>();
        int frameH = spriteNode["frameHeight"].as<int>();
        int frameTime = spriteNode["frameTime"].as<int>();

        
        AnimatedSprite* sprite = new AnimatedSprite(image, frameW, frameH);
        Character * ch = new Character(sprite);

        sprite->setFrameTime(frameTime);

        YAML::Node anims = spriteNode["animations"];
        if (!anims) throw std::runtime_error("Missing 'animations' section");

        for (auto it = anims.begin(); it != anims.end(); ++it) {
            std::string name = it->first.as<std::string>();
            int row = it->second["row"].as<int>();
            int frames = it->second["frames"].as<int>();
            sprite->addAction(name, row, frames);
        

            YAML::Node rects = it->second["BoundingBoxes"];
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
        boundingBoxes[name][frame] = r;
    }


    // Returns the bounding box in absolute co-ords
    SDL_Rect getGlobalBoundingBox(){
        return getCurrentBoundingBox() + SDL_Point{x,y};
    }

private:
    AnimatedSprite* sprite;
    // Returns the current action and frame relative bounding box
    SDL_Rect getCurrentBoundingBox(){
        return boundingBoxes[sprite->getCurrentAction()][sprite->getCurrentFrame()];
    }
    int x, y, dx,dy, speed;

    // Ordered map for Ani actions pointing to another map 
    std::unordered_map<std::string, std::unordered_map<int,SDL_Rect>> boundingBoxes;
    //SDL_Rect currentBoundingBox;
};
