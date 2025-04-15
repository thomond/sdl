#include "SDL2/SDL.h"
#include "stdio.h"

const int SCREEN_W = 640;
const int SCREEN_H = 480;

SDL_Window * window = NULL;
SDL_Surface * scrSurface = NULL;
SDL_Renderer * r = NULL;
// event flags
bool quit = false;
SDL_Event e;

// Helper functions
bool init();
bool loadMedia();
void close();
bool DrawRect();


bool init()
{
	if ( SDL_Init(SDL_INIT_VIDEO)< 0 )
	{
		printf("SDL could not initialize! SDL Error: %s\n",SDL_GetError());
		return false;
	}
	
	// Create window with dimensions 
	window = SDL_CreateWindow("SDL Test",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
	if (window != NULL) {
		r = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
		if(r == NULL){
			printf(SDL_GetError());
		}
		scrSurface = SDL_GetWindowSurface(window);
		return true;
	}
	else printf("Window could not be created",SDL_GetError());
	return false;
}

int main(){
	init();
	
	while(!quit)
	{
		// Handle input events in queue by polling them
		while(SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
				quit = true;

			// Set draw BG and clear
			SDL_SetRenderDrawColor( r, 255, 255, 255, 255 );
			SDL_RenderClear( r );

			// Set rect bg
			SDL_SetRenderDrawColor( r, 255, 0, 0, 255 );	
			SDL_Rect rect = { 100,100,32,32};
			SDL_RenderFillRect(r,&rect);

			SDL_RenderPresent(r);
			SDL_Delay( 5 );
		}


	}
	close();
	return 0;
	//SDL_Rect rect = {.x = 10,.y = 10,.w = 32,.h = 32};

}

void close()
{
	// Shutdown SDL
	SDL_DestroyWindow( window );
	SDL_Quit();
}
