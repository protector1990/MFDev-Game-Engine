#include "Lua.h"
#include "lua-5.3.2\src\lstate.h"
#include <iostream>

void luaBind(lua_State *interpreter) {
	lua_register(interpreter, "test", &luaTest);
}

void luaExecute(lua_State *interpreter, LuaScript *script) {
	luaL_loadbufferx(interpreter, script->_contents, script->_size, script->_name, NULL);
	lua_pcall(interpreter, 0, LUA_MULTRET, 0);
}

int luaTest(lua_State *state) {
	std::cout << "test working";
	return 0;
}