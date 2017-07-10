/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Common.h"
#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_platform.h>
#include "InputManager.h"
#include "PriorityQueue.h"

void printQueue(PriorityQueue<int>* arg) {
	//for (size_t i = 1; i <= arg->size(); ++i)
	//{
	//	printf("%i ", arg->_array[i]);
	//}
	//printf("\n");
}

void testbed() {
	//PriorityQueue<int> queue;
	//queue.insert(5);
	//printQueue(&queue);
	//queue.insert(7);
	//printQueue(&queue);
	//queue.insert(3);
	//printQueue(&queue);
	//queue.insert(1);
	//printQueue(&queue);
	//queue.insert(6);
	//printQueue(&queue);
	//queue.insert(9);
	//printQueue(&queue);
	//queue.insert(4);
	//printQueue(&queue);
	//queue.insert(11);
	//printQueue(&queue);
	//queue.pop();
	//printQueue(&queue);
	//system("pause");
}

// The architecture and location of the main loop will maybe have to change to add support for hot realoding and restarting the game
int Engine::run(int argv, char** argc) {
#ifdef _DEBUG
	testbed();
#endif

#ifdef __WINDOWS__
	_assetManager = new WinAssetManager();
#endif
#ifdef __IOS__
	_assetManager = new IOSAssetManager();
#endif;
	_assetManager->init();
	
	_luaManager = new LuaManager;
	_luaManager->initManager();
	
	_video = new Video();
	_video->init();

	_inputManager = new InputManager();

	//TODO: make scenes manager class
	_configurationManager = new ConfigurationManager();
	_configurationManager->init();

	// Scene with index 0 is always the first scene engine loads. Should enable passing it as a command line argument in debug mode
	Scene* firstScene = ASSET_MANAGER->loadAsset<Scene>(_configurationManager->getScenePath(0));
	_scenes.push_back(firstScene);
	firstScene->init();
	firstScene->activate();

	SDL_Event sdlEvent;
	while (_running) {
		while (SDL_PollEvent(&sdlEvent)) {
			switch (sdlEvent.type) {
			case SDL_QUIT:
				{
					_running = false;
					break;
				}
			case SDL_KEYDOWN:
				{
					int keyDown = sdlEvent.key.keysym.sym;
					for (unsigned int i = 0; i < _scenes.size(); i++)
					{
						_scenes[i]->keyPressed(keyDown);
					}
					_inputManager->keyPressed(sdlEvent.key.keysym.sym);
					break;
				}
			case SDL_KEYUP:
				{
					int keyDown = sdlEvent.key.keysym.sym;
					for (unsigned int i = 0; i < _scenes.size(); i++)
					{
						_scenes[i]->keyPressed(keyDown);
					}
					_inputManager->keyReleased(sdlEvent.key.keysym.sym);
					break;
				}
			case SDL_MOUSEBUTTONDOWN:
				{
					_inputManager->mousePressed('c');
					break;
				}
			case SDL_MOUSEBUTTONUP:
				{
					_inputManager->mouseReleased('c');
					break;
				}
			case SDL_MOUSEMOTION:
				{
					_inputManager->changeMousePos(glm::vec2(sdlEvent.motion.x - 320, -sdlEvent.motion.y + 192));
					break;
				}
			}
		}
		_frameTime = SDL_GetTicks();
		_deltaTime = (_frameTime - _lastFrameTime) / 1000.f;
		for (unsigned int i = 0; i < _scenes.size(); i++) {
			if (_scenes[i]->getActive()) {
				_scenes[i]->update(_deltaTime);
			}
		}

		_video->render();

		_lastFrameTime = _frameTime;
	}
	_assetManager->deInit();
	SDL_Quit();
	return 0;
}

void Engine::renderScenes() {
	
}

Engine::Engine(): _assetManager(nullptr), _inputManager(nullptr), _deltaTime(0) {}

Engine::~Engine() {

}

void Engine::quit() {
	_running = false;
}
