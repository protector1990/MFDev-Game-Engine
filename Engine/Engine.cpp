/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Common.h"
#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_platform.h>
#include "InputManager.h"

int Engine::run() {
	_luaInterpreter = luaL_newstate();
	luaL_openlibs(_luaInterpreter);
	LuaManager::luaBind(_luaInterpreter);

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

	_inputManager = new InputManager();

	Scene* testScene = ASSET_MANAGER->loadAsset<Scene>("/scenes/TestScene/TestScene.level");
	_scenes.insert(_scenes.end(), testScene);
	testScene->init();
	testScene->activate();

	//loadConfiguration();

	SDL_Event sdlEvent;
	while (_running) {
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT) {
				_running = false;
				break;
			}
			if (sdlEvent.type == SDL_KEYDOWN){
				int keyDown = sdlEvent.key.keysym.sym;
				for (unsigned int i = 0; i < _scenes.size(); i++){
					_scenes[i]->keyPressed(keyDown);
				}
				_inputManager->keyPressed(sdlEvent.key.keysym.sym);
			}
			if (sdlEvent.type == SDL_KEYUP){
				int keyDown = sdlEvent.key.keysym.sym;
				for (unsigned int i = 0; i < _scenes.size(); i++){
					_scenes[i]->keyPressed(keyDown);
				}
				_inputManager->keyReleased(sdlEvent.key.keysym.sym);
			}
		}
		_frameTime = SDL_GetTicks();
		_deltaTime = (_frameTime - _lastFrameTime) / 1000.f;
		for (unsigned int i = 0; i < _scenes.size(); i++) {
			if (_scenes[i]->getActive()) {
				_scenes[i]->update(_deltaTime);
			}
		}

		//Currently only calls Engine::renderScenes()
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
