// main engine class
#include "LuaBindings.h"
#include "Renderer.h"

class Engine {
public:
	lua_State *_luaInterpreter;
	Renderer *_renderer;
	int run();
};