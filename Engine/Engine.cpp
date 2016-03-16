#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_platform.h>
//#include "..\libs\physfs-2.0.3\physfs.h"

int Engine::run() {

	_luaInterpreter = luaL_newstate();
	luaBind(_luaInterpreter);

	SDL_Init(0);

#ifdef __WINDOWS__
	_assetManager = new WinAssetManager();
#endif
#ifdef __IOS__
	_assetManager = new IOSAssetManager();
#endif;
	_assetManager->init();

	_renderer = new Renderer();
	_renderer->init();

	//test code
	LuaScript* script = _assetManager->loadAsset<LuaScript>("/llua.lua");
	luaExecute(_luaInterpreter, script);

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

Engine::Engine() {

}

Engine::~Engine() {

}

void Engine::quit() {
	_running = false;
}