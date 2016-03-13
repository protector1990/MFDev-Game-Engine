#include "Engine.h"

int Engine::run() {
	_luaInterpreter = luaL_newstate();
	luaBind(_luaInterpreter);
	return 0;
}