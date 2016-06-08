#include "Lua.h"
#include "lua-5.3.2\src\lstate.h"
#include <iostream>
#include "Common.h"
#include "InputManager.h"

ScriptComponent::ScriptComponent(Script *script, GameObject *parentObject) {
	_script = script;

	lua_newtable(ENGINE.getLuaInterpreter());
	//lua_rawgeti(ENGINE.getLuaInterpreter(), LUA_REGISTRYINDEX, _reference);
	lua_pushlightuserdata(ENGINE.getLuaInterpreter(), parentObject);
	//printf("\n%i %i\n", lua_tonumber(ENGINE.getLuaInterpreter(), -1), (long)parentObject);
	lua_setfield(ENGINE.getLuaInterpreter(), -2, "gameObject");
	// lua_pop(ENGINE.getLuaInterpreter(), 2);

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
	lua_register(interpreter, "translate", &luaTranslate);
	lua_register(interpreter, "queryKeyDown", &luaQueryKeyDown);
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

	script->reference = luaL_ref(interpreter, LUA_REGISTRYINDEX);
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

int LuaManager::luaTranslate(lua_State *state) {
	GameObject* targetObject = (GameObject*)lua_touserdata(state, -4);
	float x = lua_tonumber(state, -3);
	float y = lua_tonumber(state, -2);
	float z = lua_tonumber(state, -1);
	targetObject->_Position.x += x;
	targetObject->_Position.y += y;
	targetObject->_Position.z += z;
	return 0;
}

int LuaManager::luaQueryKeyDown(lua_State *state) {
	char key = lua_tonumber(state, -1);
	lua_pushboolean(state, ENGINE.getInputManager()->queryKeyDown(key));
	return 1;
}