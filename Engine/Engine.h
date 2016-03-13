// main engine class
#include "LuaBindings.h"
#include "Renderer.h"
#include <SDL.h>

class Engine {
public:
	lua_State *_luaInterpreter;
	Renderer *_renderer;
	void quit();
	int run();
private:
	bool _running = true;
	float _deltaTime;
	// TODO: make getters
	Uint32 _frameTime = 0;
	Uint32 _lastFrameTime = 0;
};