#include "Common.h"
#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_platform.h>

//temp, make this platform independant or remove altogether
#include <windows.h>

int Engine::run() {
	bool firstTime = true;
	_luaInterpreter = luaL_newstate();
	luaBind(_luaInterpreter);
	//Sleep(1000);

	SDL_Init(SDL_INIT_VIDEO);
	Sleep(1500);
#ifdef __WINDOWS__
	_assetManager = new WinAssetManager();
#endif
#ifdef __IOS__
	_assetManager = new IOSAssetManager();
#endif;
	_assetManager->init();

	//Sleep(1000);

	_renderer = new Renderer();
	_renderer->init();
	//Sleep(1000);
	Scene* testScene = ASSET_MANAGER->loadAsset<Scene>("/scenes/TestScene/TestScene.level");
	_scenes.insert(_scenes.end(), testScene);
	//Sleep(1000);
	testScene->init();
	//Sleep(1000);
	testScene->activate();

	//loadConfiguration();

	SDL_Event quitEvent;
	while (_running) {
		while (SDL_PollEvent(&quitEvent)) {
			if (quitEvent.type == SDL_QUIT) {
				_running = false;
				break;
			}
		}
		if (firstTime) {
			Sleep(2000);
			firstTime = false;
		}
		_frameTime = SDL_GetTicks();
		_deltaTime = (_frameTime - _lastFrameTime) / 1000.f;
		for (unsigned int i = 0; i < _scenes.size(); i++) {
			if (_scenes[i]->getActive()) {
				_scenes[i]->update(_deltaTime);
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

void Engine::renderScenes() {
	for (unsigned int i = 0; i < _scenes.size(); i++) {
		if (_scenes[i]->getActive()) {
			_scenes[i]->render(_renderer);
		}
	}
}

Engine::Engine() {

}

Engine::~Engine() {

}

void Engine::quit() {
	_running = false;
}
