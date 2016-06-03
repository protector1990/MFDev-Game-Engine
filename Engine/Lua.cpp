#include "Lua.h"
#include "lua-5.3.2\src\lstate.h"
#include <iostream>
#include "Common.h"

ScriptComponent::ScriptComponent(Script *script) {
	_script = script;

	lua_newtable(ENGINE.getLuaInterpreter());
	_reference = luaL_ref(ENGINE.getLuaInterpreter(), LUA_REGISTRYINDEX);
}

int ScriptComponent::getReference() {
	return _reference;
}

Script* ScriptComponent::getScript() {
	return _script;
}

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
void LuaManager::luaParse(lua_State *interpreter, Script *script) {
	int res = luaL_loadbufferx(interpreter, script->_contents, script->_size, script->_name, NULL);
	if (lua_isstring(interpreter, -1)) {
		printf("%s", lua_tostring(interpreter, -1));
	}
	if (!lua_isfunction(interpreter, -1))
	{
		printf("\n\n[C++]: Oops! The lua script '%s' has not been defined", script->_name);
	}
	lua_pcall(interpreter, 0, 0, 0);

	lua_getglobal(interpreter, "currentClass");
	const char *className = lua_tostring(interpreter, -1);

	lua_getglobal(interpreter, className);
	if (lua_istable(interpreter, -1)) {
		printf("istable");
	}
	script->reference = luaL_ref(interpreter, LUA_REGISTRYINDEX);

//	lua_pop(interpreter, 3);
	//lua_err
	//luaL_loadstring(interpreter, script->_contents);
	//luaL_dofile(interpreter, "data/scripts/player.lua");
}

void LuaManager::luaCall(lua_State *interpreter, ScriptComponent *component, const char* name, float* params, int paramsNum) {
	//lua_settop(interpreter, 0);
	if (!component) {
		return;
	}

	//lua_settop(interpreter, 0);
	int reference = component->getScript()->reference;
	lua_rawgeti(interpreter, LUA_REGISTRYINDEX, component->getScript()->reference);
	//if (lua_istable(interpreter, -1)) {
	//	printf("%i table reference referenced properly", LUA_REGISTRYINDEX);
	//}
	
	int x = lua_getfield(interpreter, -1, name);

	if (!lua_isfunction(interpreter, -1)) {
		printf("\nnot defined.");
	}

	//lua_gettable(interpreter, -2);
	//lua_insert(interpreter, -2);

	if (!lua_isfunction(interpreter, -1))
	{
		printf("\n\n[C++]: Oops! The lua function '%s' has not been defined", name);
	}
	else {
		for (int i = 0; i < paramsNum; i++) {
			//printf("\n%f\n%f\n", *(params + i), params[i]);
			lua_pushnumber(interpreter, params[i]);
		}
		//int b = lua_type(interpreter, -2);
		lua_rawgeti(interpreter, LUA_REGISTRYINDEX, component->getReference());
		//int a = lua_type(interpreter, -1);
		//int b = lua_type(interpreter, -2);
		//int c = lua_type(interpreter, -3);
		lua_insert(interpreter, -2);
		lua_call(interpreter, paramsNum+1, 0);
		lua_pop(interpreter, paramsNum);
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