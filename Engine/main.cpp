#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
	SDL_Init(0);
	SDL_Window *gameWindow;
	gameWindow = SDL_CreateWindow(
		"Prozor igre",
		200, 200,
		450, 300,
		SDL_WINDOW_OPENGL);
	//system("pause");
	SDL_Delay(5000);
	SDL_Quit();
	return 0;
}