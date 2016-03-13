#include "Renderer.h"
#include <SDL.h>
#include <SDL_opengl.h>

using namespace std;

SDL_Window *gameWindow = 0;
SDL_Surface *screenSurface = 0;
SDL_Surface *image = 0;

//helper function
void LoadMedia(){
	image = SDL_LoadBMP("ZnamSDL.bmp");
}

void Renderer::init() {
	//renderer initialization
	gameWindow = SDL_CreateWindow("Game Window", 300, 150, 400, 400, SDL_WINDOW_SHOWN);
	screenSurface = SDL_GetWindowSurface(gameWindow);
	LoadMedia();
}

void Renderer::render(float deltaTime) {
	SDL_BlitSurface(image, 0, screenSurface, 0);
	SDL_UpdateWindowSurface(gameWindow);
}