// main engine class
#include "LuaBindings.h"

class Engine {
public:
	lua_State *_luaInterpreter;
	int run();
};