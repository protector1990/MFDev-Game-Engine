// main engine class
#include "Renderer.h"
#include <SDL.h>
#include "AssetManager.h"

class Engine {
public:
	~Engine();
	void quit();
	int run();
	lua_State* getLuaInterpreter() { return _luaInterpreter; }
	Renderer* getRenderer() { return _renderer; }
	AssetManager* getAssetManager() { return _assetManager; }

	static Engine& getInstance() {
		static Engine instance;
		return instance;
	}

private:
	Engine();

	lua_State * _luaInterpreter;
	Renderer * _renderer;
	AssetManager * _assetManager;

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