/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Lua.h"
#include <lua-5.3.2/src/lstate.h>
#include <iostream>
#include "Common.h"
#include "InputManager.h"
#include "ExposedFunctions.h"

static void stackDump(lua_State *L) {
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {

		case LUA_TSTRING:  /* strings */
			printf("`%s'", lua_tostring(L, i));
			break;

		case LUA_TBOOLEAN:  /* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;

		case LUA_TNUMBER:  /* numbers */
			printf("%g", lua_tonumber(L, i));
			break;

		default:  /* other values */
			printf("%s", lua_typename(L, t));
			break;

		}
		printf("  ");  /* put a separator */
	}
	printf("\n");  /* end the listing */
}

#define DUMP stackDump(SCRIPT_MANAGER->getLuaInterpreter());

Script* ScriptClass::getScript() const {
	return script;
}

void ScriptClass::setScript(Script* script) {
	this->script = script;
}

int ScriptClass::getReference() const {
	return _reference;
}

void ScriptClass::setReference(int reference) {
	_reference = reference;
}

ScriptComponent::ScriptComponent(ScriptClass *scriptClass, GameObject *parentObject) : _script(scriptClass) {
	lua_State* interpreter = SCRIPT_MANAGER->getLuaInterpreter();

	lua_newtable(interpreter);

	int tb = lua_rawgeti(interpreter, LUA_REGISTRYINDEX, scriptClass->getReference());
	lua_pushnil(interpreter);

	while (lua_next(interpreter, -2)) {
		lua_setfield(interpreter, -4, lua_tostring(interpreter, -2));
	}
	//DUMP printf("1\n");

	lua_pop(interpreter, 1);
	//DUMP printf("2\n");

	const type_info &a = typeid(*parentObject);

	// Accessor table
	lua_getglobal(interpreter, "Accessor");
	SCRIPT_MANAGER->luaCopyTable(-1);
	//DUMP

	int exposedFunctions = GetExposedFunctionsForType(a);
	if (exposedFunctions) {
		lua_rawgeti(interpreter, LUA_REGISTRYINDEX, GetExposedFunctionsForType(a));
		lua_setfield(interpreter, -2, "_tb");
	}
	//DUMP

	//lua_pop(interpreter, 1);

	lua_pushlightuserdata(interpreter, parentObject);
	lua_setfield(interpreter, -2, "ref");

	lua_setfield(interpreter, -3, a.name());
	lua_pop(interpreter, 1);
	//DUMP

	DUMP
	

	_reference = luaL_ref(interpreter, LUA_REGISTRYINDEX);
	DUMP
}

void LuaManager::luaCopyTable(int index) {
	lua_newtable(_luaInterpreter);
	lua_pushnil(_luaInterpreter);
	while (lua_next(_luaInterpreter, index - 2)) {
		lua_setfield(_luaInterpreter, -3, lua_tostring(_luaInterpreter, -2));
	}
}

int ScriptComponent::getReference() const {
	return _reference;
}

ScriptClass* ScriptComponent::getScriptClass() const {
	return _script;
}

int luaSample(lua_State* a) {
	return 1;
}

void LuaManager::luaBind() const {
	//luaL_openlibs(interpreter);
	lua_register(_luaInterpreter, "test", &luaTest);
	lua_register(_luaInterpreter, "mprint", &luaPrint);
	lua_register(_luaInterpreter, "translate", &luaTranslate);
	lua_register(_luaInterpreter, "queryKeyDown", &luaQueryKeyDown);
	lua_register(_luaInterpreter, "getPosition", &luaGetPosition);
	lua_register(_luaInterpreter, "setPosition", &luaSetPosition);
	lua_register(_luaInterpreter, "luaSample", &luaSample);
}

//TODO: Clean this up and properly test it. It's bound to have some leaks
int LuaManager::luaParseComponent(lua_State *interpreter, Script *script) {
	int res = luaL_loadbufferx(interpreter, script->_contents, script->_size, script->_name, nullptr);
	if (!lua_isfunction(interpreter, -1))
	{
		printf("[Lua]: Parsing failed for script {%s}: %s", script->_name, lua_tostring(interpreter, -1));
		return LUA_NOREF;
	}
	lua_pcall(interpreter, 0, 0, 0);

	lua_getglobal(interpreter, "currentClass");
	const char *className = lua_tostring(interpreter, -1);

	lua_getglobal(interpreter, className);
	if (lua_istable(interpreter, -1)) {

		lua_getglobal(interpreter, "extends");
		const char *extends = lua_tostring(interpreter, -1);
		if ((extends) && (strlen(extends) > 0)) {

			lua_pop(interpreter, 1);

			lua_getglobal(interpreter, extends);

			if (!lua_istable(interpreter, -1)) {
				printf("[Lua]: Parent class not loaded: {%s}, attempting to load...\n", extends);
				char path[64];
				strcpy(path, "/scripts/");
				strcat(path, extends);
				strcat(path, ".lua");
				//TODO: Break mutual dependancies between lua manager and asset manager
				ASSET_MANAGER->loadAsset<ScriptClass>(path);
				lua_pop(interpreter, 1);
				lua_getglobal(interpreter, extends);
				if (!lua_istable(interpreter, -1)) {
					printf("\nLua Parsing Error: lua script not found: {%s}!\n", path);
					lua_pop(interpreter, 1);
					return LUA_NOREF;
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
		//script->reference = luaL_ref(interpreter, LUA_REGISTRYINDEX);
		printf("[Lua]: Successfully loaded: {%s}\n", className);
		return luaL_ref(interpreter, LUA_REGISTRYINDEX);
	}
#if defined(_DEBUG)
	else {
		printf("[Lua]: Parsing Error: class not found: {%s}!", className);
		return LUA_NOREF;
	}
#endif
}

void LuaManager::luaParsePlainScript(lua_State* interpreter, Script* script)
{

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

void LuaManager::initManager() {
	_luaInterpreter = luaL_newstate();
	luaL_openlibs(_luaInterpreter);
	luaBind();
	// TODO: Make these non static
	Script* metaTableScript = ASSET_MANAGER->loadAsset<Script>("scripts/system/_lua_Game_object.lua");
	int res = luaL_loadbufferx(_luaInterpreter, metaTableScript->_contents, metaTableScript->_size, metaTableScript->_name, nullptr);
	if (!lua_isfunction(_luaInterpreter, -1))
	{
		printf("[Lua]: Parsing failed for script {%s}", metaTableScript->_name);
		return;
	}
	lua_call(_luaInterpreter, 0, -1);
	lua_getglobal(_luaInterpreter, "Accessor");
	if (!lua_istable(_luaInterpreter, -1))
	{
		printf("[Lua]: Acessor table not found!\n");
		return;
	}
	_gameObjectMetaTable = luaL_ref(_luaInterpreter, LUA_REGISTRYINDEX);
	ExposeFunctionsToScript();
}

lua_State* LuaManager::getLuaInterpreter() const {
	return _luaInterpreter;
}
