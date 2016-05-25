#include "Lua.h"
#include "lua-5.3.2\src\lstate.h"
#include <iostream>

void LuaManager::luaBind(lua_State *interpreter) {
	luaL_openlibs(interpreter);
	lua_register(interpreter, "test", &luaTest);
	lua_register(interpreter, "mprint", &luaPrint);
	lua_register(interpreter, "setPosition", &luaSetPosition);
}

//calling lua from c++
void LuaManager::luaExecute(lua_State *interpreter, Script *script) {
	int res = luaL_loadbufferx(interpreter, script->_contents, script->_size, script->_name, NULL);
	lua_pcall(interpreter, 0, LUA_MULTRET, 0);
	//luaL_dofile(interpreter, "some");
}

ScriptComponent* LuaManager::luaNewObject(lua_State *interpreter, Script *script) {
	int res = luaL_loadbufferx(interpreter, script->_contents, script->_size, script->_name, NULL);
	if (!lua_isfunction(interpreter, -1))
	{
		printf("\n\n[C++]: Oops! The lua function '%s' has not been defined", script->_name);
	}
	lua_pcall(interpreter, 0, 0, 0);
	//lua_err
	//luaL_loadstring(interpreter, script->_contents);
	//luaL_dofile(interpreter, "data/scripts/player.lua");
}

void LuaManager::luaCall(lua_State *interpreter, Script *script, const char* name, int* params, int paramsNum) {
	//lua_settop(interpreter, 0);
	if (!script) {
		return;
	}
	
	int x = lua_getglobal(interpreter, name);
	if (!lua_isfunction(interpreter, -1))
	{
		printf("\n\n[C++]: Oops! The lua function '%s' has not been defined", name);
	}
	else {
		for (int i = 0; i < paramsNum; i++) {
			lua_pushnumber(interpreter, *(params + i));
		}
		lua_call(interpreter, paramsNum, 0);
		//int x = lua_pcall(interpreter, 0, LUA_MULTRET, 0);
	}
}

//c++ callbacks from lua
int LuaManager::luaTest(lua_State *state) {
	std::cout << "test working";
	return 0;
}

int LuaManager::luaPrint(lua_State *state) {
	std::cout << (state, lua_gettop(state));
	return 0;
}

int LuaManager::luaSetPosition(lua_State *state) {
	return 0;
}