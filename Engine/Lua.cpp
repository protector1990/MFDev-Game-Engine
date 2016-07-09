/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Lua.h"
#include "lua-5.3.2\src\lstate.h"
#include <iostream>
#include "Common.h"
#include "InputManager.h"

ScriptComponent::ScriptComponent(Script *script, GameObject *parentObject) {
	_script = script;
	lua_State* interpreter = ENGINE.getLuaInterpreter();

	lua_newtable(interpreter);

	int tb = lua_rawgeti(interpreter, LUA_REGISTRYINDEX, script->reference);
	lua_pushnil(interpreter);

	while (lua_next(interpreter, -2)) {
		lua_setfield(interpreter, -4, lua_tostring(interpreter, -2));
	}

	lua_pop(interpreter, 1);

	lua_pushlightuserdata(interpreter, parentObject);
	lua_setfield(interpreter, -2, "gameObject");

	_reference = luaL_ref(interpreter, LUA_REGISTRYINDEX);
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
	lua_register(interpreter, "getPosition", &luaGetPosition);
	lua_register(interpreter, "setPosition", &luaSetPosition);
}

void LuaManager::luaExecute(lua_State *interpreter, Script *script) {
	int res = luaL_loadbufferx(interpreter, script->_contents, script->_size, script->_name, nullptr);
	lua_pcall(interpreter, 0, LUA_MULTRET, 0);
}

//TODO: Clean this up and properly test it. It's bound to have some leaks
void LuaManager::luaParse(lua_State *interpreter, Script *script) {
	int res = luaL_loadbufferx(interpreter, script->_contents, script->_size, script->_name, nullptr);
	if (!lua_isfunction(interpreter, -1))
	{
		printf("[Lua]: Parsing failed for script {%s}", script->_name);
		return;
	}
	lua_pcall(interpreter, 0, 0, 0);

	lua_getglobal(interpreter, "currentClass");
	const char *className = lua_tostring(interpreter, -1);

	lua_getglobal(interpreter, className);
	if (lua_istable(interpreter, -1)) {

		lua_getglobal(interpreter, "extends");
		const char *extends = lua_tostring(interpreter, -1);
		if (strlen(extends) > 0) {

			lua_pop(interpreter, 1);

			lua_getglobal(interpreter, extends);

			if (!lua_istable(interpreter, -1)) {
				printf("[Lua]: Parent class not loaded: {%s}, attempting to load...\n", extends);
				char* path = new char[64];
				strcpy(path, "/scripts/");
				strcat(path, extends);
				strcat(path, ".lua");
				ASSET_MANAGER->loadAsset<Script>(path);
				lua_pop(interpreter, 1);
				lua_getglobal(interpreter, extends);
				if (!lua_istable(interpreter, -1)) {
					printf("\nLua Parsing Error: lua script not found: {%s}!\n", path);
					lua_pop(interpreter, 1);
					return;
				}
			}

			//maybe lua rotate trick?
			lua_pushnil(interpreter);
			while (lua_next(interpreter, -2)) {
				const char* field = lua_tostring(interpreter, -2);
				int x = lua_getfield(interpreter, -5, field);
				if (x == LUA_TNIL) {
					lua_pop(interpreter, 1);
					lua_setfield(interpreter, -5, field);
				}
				else {
					lua_pop(interpreter, 2);
				}
			}

			lua_pop(interpreter, 2);
		}
		else {
			lua_pop(interpreter, 1);
		}
		const char * x1 = lua_tostring(interpreter, -1);
		script->reference = luaL_ref(interpreter, LUA_REGISTRYINDEX);
		printf("[Lua]: Successfully loaded: {%s}\n", className);
	}
#if defined(_DEBUG)
	else {
		printf("[Lua]: Parsing Error: class not found: {%s}!", className);
	}
#endif
}

void LuaManager::luaCall(lua_State *interpreter, ScriptComponent *component, const char* name, float* params, int paramsNum) {
	if (!component) {
		return;
	}

	lua_rawgeti(interpreter, LUA_REGISTRYINDEX, component->getReference());

	lua_getfield(interpreter, -1, name);

	if (!lua_isfunction(interpreter, -1))
	{
		printf("[Lua]: The lua function {%s} not defined", name);
	}
	else {
		for (int i = 0; i < paramsNum; i++) {
			lua_pushnumber(interpreter, params[i]);
		}
		lua_rawgeti(interpreter, LUA_REGISTRYINDEX, component->getReference());
		lua_insert(interpreter, -2);
		lua_call(interpreter, paramsNum+1, 0);
		lua_pop(interpreter, paramsNum);
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
	GameObject* targetObject = static_cast<GameObject*>(lua_touserdata(state, -4));
	float x = lua_tonumber(state, -3);
	float y = lua_tonumber(state, -2);
	float z = lua_tonumber(state, -1);
	targetObject->_Position.x += x;
	targetObject->_Position.y += y;
	targetObject->_Position.z += z;
	return 0;
}

int LuaManager::luaSetPosition(lua_State *state) {
	GameObject* targetObject = static_cast<GameObject*>(lua_touserdata(state, -4));
	float x = lua_tonumber(state, -3);
	float y = lua_tonumber(state, -2);
	float z = lua_tonumber(state, -1);
	targetObject->_Position.x = x;
	targetObject->_Position.y = y;
	targetObject->_Position.z = z;
	return 0;
}

int LuaManager::luaGetPosition(lua_State *state) {
	GameObject* targetObject = static_cast<GameObject*>(lua_touserdata(state, -1));
	lua_pushnumber(state, targetObject->_Position.x);
	lua_pushnumber(state, targetObject->_Position.y);
	lua_pushnumber(state, targetObject->_Position.z);
	return 3;
}

int LuaManager::luaQueryKeyDown(lua_State *state) {
	char key = lua_tonumber(state, -1);
	lua_pushboolean(state, ENGINE.getInputManager()->queryKeyDown(key));
	return 1;
}

void LuaManager::initManager(lua_State *interpreter) {
	
}