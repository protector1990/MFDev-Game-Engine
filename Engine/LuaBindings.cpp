#include "LuaBindings.h"
#include <iostream>

void luaBind(lua_State *interpreter) {
	lua_register(interpreter, "test", &luaTest);
}

int luaTest(lua_State *state) {
	std::cout << "test working";
	return 0;
}