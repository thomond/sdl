#include "AnimatedSprite.hpp"
#include "Character.hpp"
int main() {
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);
  SDL_Window *win = SDL_CreateWindow("Animated Sprite", 100, 100, 800, 600, 0);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  Character * character = Character::createFromYaml(renderer, "player.yaml");
  
  AnimatedSprite sprite(renderer, "Unarmed_Walk_full.png", 64, 64);
  Character character(sprite);

  sprite.addAction("walk_down", 0, 6);  // row 0, 4 frames
  sprite.addAction("walk_left", 1, 6);  // row 1, 6 frames
  sprite.addAction("walk_right", 2, 6); // row 2, 8 frames
  sprite.addAction("walk_up", 3, 6); // row 2, 8 frames
  sprite.setAction("walk_down");
  sprite.setFrameTime(120); // ms per frame

  bool running = true;
  SDL_Event e;
  while (running) {
    Uint32 now = SDL_GetTicks();
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        running = false;
      if (e.type == SDL_KEYDOWN) {
        character.handleInput(e.key.keysym.sym);
        switch (e.key.keysym.sym) {
        case SDLK_q:
          running = false;
          break;
        }
      }
    }

    character.update(now);

    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderClear(renderer);
    character.draw();
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  IMG_Quit();
  SDL_Quit();
}
