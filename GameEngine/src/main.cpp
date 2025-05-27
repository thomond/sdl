#define DEBUG
#include "helpers/SDL_RectMath.hpp"
#include "Sprites/AnimatedSprite.hpp"
#include "Sprites/Character.hpp"
#include "Renderer.cpp"
#include "Sprites/TileGroup.hpp"
#include "Tilesheet.hpp"
int main() {
 
  auto rend = Renderer::init();

  auto  Tilesheet = Tilesheet::createFromYaml("res/Tilesheet.yaml");
  auto tiles = TileGroup(Tilesheet);
  tiles.addTile("wall",{0,0});
  tiles.addTile("wall",{0,32});
  tiles.addTile("wall",{0,64});
  tiles.addTile("player",{128,128});


  Renderer::addToRenderList(&tiles);
  
  bool running = true;
  SDL_Event e;
  while (running) {
    Uint32 now = SDL_GetTicks();
    SDL_Point delta = {0,0};
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        running = false;
      if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
        case SDLK_d:
          delta.x = 1;
        break;
        case SDLK_w:
          delta.x = -1;
        break;
        case SDLK_a:
          delta.y = 1;
        break;
        case SDLK_s:
          delta.y = -1;
        break;
        case SDLK_q:
          running = false;
          break;
        }
      }
    }

    // 2) Collision check BEFORE committing movement:
    if (tiles.collidesWith("player","wall", delta)) {
        auto player = tiles.getTile("player");
        player->tile += delta;
    } else {
        // Optionally respond to collision, e.g. stop movement:
        delta = {0,0};
    }

    Renderer::draw();
  }

  Renderer::destroy();
}

void test_Tilemap() {
    auto tilesheet = Tilesheet::createFromYaml("res/tilesheet.yaml");
    auto tilemapData = TileMapper::loadTilemapData("res/tilemap.yaml");

    SDL_Point layerDim = {800, 600};
    TileMapper::renderTilemap(layerDim, tilesheet, tilemapData);
}