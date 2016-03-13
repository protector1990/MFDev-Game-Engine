#include "Renderer.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <iostream>

using namespace std;

SDL_Window *gameWindow = 0;
SDL_Surface *screenSurface = 0;
SDL_Surface *image = 0;



void Renderer::init() {
	//renderer initialization
	SDL_Init(0);
	gameWindow = SDL_CreateWindow("Game Window", 300, 150, 400, 400, SDL_WINDOW_SHOWN);
	screenSurface = SDL_GetWindowSurface(gameWindow);
}

//helper function
void LoadMedia(){
	image = SDL_LoadBMP("ZnamSDL.bmp");
}

void Renderer::render() {
	//renderer one frame
	LoadMedia();
	SDL_BlitSurface(image, 0, screenSurface, 0);
	SDL_UpdateWindowSurface(gameWindow);
	SDL_Delay(5000);
	SDL_DestroyWindow(gameWindow);
	SDL_Quit();
}