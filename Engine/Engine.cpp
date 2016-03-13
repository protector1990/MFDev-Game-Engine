#include "Engine.h"
#include <iostream>
#include <SDL.h>

int Engine::run() {
	_luaInterpreter = luaL_newstate();
	luaBind(_luaInterpreter);
	luaL_dofile(_luaInterpreter, "llua.lua");
	SDL_Init(0);
	_renderer = new Renderer();
	_renderer->init();
	//this should be called from a loop when we make one
	SDL_Event quitEvent;
	while (_running) {
		while (SDL_PollEvent(&quitEvent)) {
			if (quitEvent.type == SDL_QUIT){
				_running = false;
				break;
			}
		}
		_frameTime = SDL_GetTicks();
		_deltaTime = (_frameTime - _lastFrameTime) / 1000.f;
		_lastFrameTime = _frameTime;
		_renderer->render(_deltaTime);
	}
	SDL_Quit();
	return 0;
}

void Engine::quit() {
	_running = false;
}