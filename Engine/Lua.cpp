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
		//lua_pop(interpreter, 1);
	}

	lua_pop(interpreter, 1);

	//lua_rawgeti(ENGINE.getLuaInterpreter(), LUA_REGISTRYINDEX, _reference);
	lua_pushlightuserdata(interpreter, parentObject);
	//printf("\n%i %i\n", lua_tonumber(ENGINE.getLuaInterpreter(), -1), (long)parentObject);
	lua_setfield(interpreter, -2, "gameObject");
	// lua_pop(ENGINE.getLuaInterpreter(), 2);

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

//calling lua from c++
void LuaManager::luaExecute(lua_State *interpreter, Script *script) {
	int res = luaL_loadbufferx(interpreter, script->_contents, script->_size, script->_name, NULL);
	lua_pcall(interpreter, 0, LUA_MULTRET, 0);
	//luaL_dofile(interpreter, "some");
}
void LuaManager::luaParse(lua_State *interpreter, Script *script) {
	int res = luaL_loadbufferx(interpreter, script->_contents, script->_size, script->_name, NULL);
	if (!lua_isfunction(interpreter, -1))
	{
		printf("\n\n[C++]: Oops! The lua script '%s' has not been defined", script->_name);
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
				printf("\nLua Parsing Error: Parent class not found: %s!\n", extends);
				char* path = new char[64];
				strcpy(path, "/scripts/");
				strcat(path, extends);
				strcat(path, ".lua");
				ASSET_MANAGER->loadAsset<Script>(path);
				lua_pop(interpreter, 1);
				lua_getglobal(interpreter, extends);
				if (!lua_istable(interpreter, -1)) {
					printf("\nLua Parsing Error: lua script not found: %s!\n", path);
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
					printf("\nDoesn't exist: %s", field);
					lua_pop(interpreter, 1);
					lua_setfield(interpreter, -5, field);
					//lua_pop(interpreter, 1);
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
	}
#if defined(_DEBUG)
	else {
		printf("\nLua Parsing Error: class not found: %s!", className);
	}
#endif
	//lua_pop(interpreter, 1);
}

void LuaManager::luaCall(lua_State *interpreter, ScriptComponent *component, const char* name, float* params, int paramsNum) {
	//lua_settop(interpreter, 0);
	if (!component) {
		return;
	}

	//lua_settop(interpreter, 0);
	//int reference = component->getScript()->reference;
	lua_rawgeti(interpreter, LUA_REGISTRYINDEX, component->getReference());
	//if (lua_istable(interpreter, -1)) {
	//	printf("%i table reference referenced properly", LUA_REGISTRYINDEX);
	//}
	
	int x = lua_getfield(interpreter, -1, name);

	//lua_gettable(interpreter, -2);
	//lua_insert(interpreter, -2);

	if (!lua_isfunction(interpreter, -1))
	{
		//printf("\n\n[C++]: Oops! The lua function '%s' has not been defined", name);
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

int LuaManager::luaSetPosition(lua_State *state) {
	GameObject* targetObject = (GameObject*)lua_touserdata(state, -4);
	float x = lua_tonumber(state, -3);
	float y = lua_tonumber(state, -2);
	float z = lua_tonumber(state, -1);
	targetObject->_Position.x = x;
	targetObject->_Position.y = y;
	targetObject->_Position.z = z;
	return 0;
}

int LuaManager::luaGetPosition(lua_State *state) {
	GameObject* targetObject = (GameObject*)lua_touserdata(state, -1);
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