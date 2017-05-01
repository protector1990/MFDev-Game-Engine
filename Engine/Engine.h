/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#ifndef _ENGINE_H
#define _ENGINE_H

// main engine class
#include "SpriteRenderer.h"
#include <SDL.h>
#include "AssetManager.h"
//#include "Scene.h"
#include <vector>
#include "ConfigurationManager.h"
#include "Video.h"

class InputManager;

class Engine {
public:
	~Engine();
	void quit();
	int run(int argc, char** argv);
	LuaManager* getLuaManager() { return _luaManager; }
	Video* getVideo() { return _video;  }
	AssetManager* getAssetManager() { return _assetManager; }
	InputManager* getInputManager() { return _inputManager; }

	char* _startupScene;


	static Engine& getInstance() {
		static Engine instance;
		return instance;
	}

	void renderScenes();

private:
	Engine();

	//lua_State * _luaInterpreter;
	LuaManager * _luaManager;
	//SpriteRenderer * _renderer;
	AssetManager * _assetManager;
	InputManager* _inputManager;
	ConfigurationManager* _configurationManager;
	Video* _video;

	std::vector<Scene*> _scenes;

	bool _running = true;
	float _deltaTime;
	// TODO: make getters
	Uint32 _frameTime = 0;
	Uint32 _lastFrameTime = 0;

public:
	//SDL_videoInfo* sdlInfo;
	Engine(Engine const&) = delete;
	void operator=(Engine const&) = delete;
};

#endif