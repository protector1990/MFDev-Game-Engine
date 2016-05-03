#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_platform.h>
//#include "..\libs\physfs-2.0.3\physfs.h"

int Engine::run() {

	_luaInterpreter = luaL_newstate();
	luaBind(_luaInterpreter);

	SDL_Init(SDL_INIT_VIDEO);

#ifdef __WINDOWS__
	_assetManager = new WinAssetManager();
#endif
#ifdef __IOS__
	_assetManager = new IOSAssetManager();
#endif;
	_assetManager->init();

	_renderer = new Renderer();
	_renderer->init();

	SDL_Event quitEvent;
	while (_running) {
		while (SDL_PollEvent(&quitEvent)) {
			if (quitEvent.type == SDL_QUIT) {
				_running = false;
				break;
			}
		}
		_frameTime = SDL_GetTicks();
		_deltaTime = (_frameTime - _lastFrameTime) / 1000.f;
		for (unsigned int i = 0; i < _scenes.size(); i++) {
			if (_scenes[i]->getActive()) {
				_scenes[i]->update(_deltaTime);
			}
		}

		for (unsigned int i = 0; i < _scenes.size(); i++) {
			if (_scenes[i]->getActive()) {
				_scenes[i]->render(_renderer);
			}
		}

		_renderer->render(_deltaTime);

		//this should be either the last command in the loop, or the first
		_lastFrameTime = _frameTime;
	}
	_assetManager->deInit();
	SDL_Quit();
	return 0;
}

Engine::Engine() {

}

Engine::~Engine() {

}

void Engine::quit() {
	_running = false;
}