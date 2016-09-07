/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Lua.h"
#include <lua-5.3.2/src/lstate.h>
#include <iostream>
#include "Common.h"
#include "InputManager.h"
#include "ExposedFunctions.h"
#include "Component.h"
#include <map>
#include "lua-5.3.2/src/lobject.h"

using namespace std;

void luaStackDump(lua_State *L) {
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

	lua_createtable(interpreter, 0, 0);

	int tb = lua_rawgeti(interpreter, LUA_REGISTRYINDEX, scriptClass->getReference());
	lua_pushnil(interpreter);

	while (lua_next(interpreter, -2)) {
		lua_setfield(interpreter, -4, lua_tostring(interpreter, -2));
	}

	lua_pop(interpreter, 1);

	const type_info &a = typeid(*parentObject);

	lua_newtable(interpreter);
	map<const char*, lua_CFunction>* exposedFunctions = GetExposedFunctionsForType(a);
	if (exposedFunctions) {
		for (pair<const char*, lua_CFunction> iv : *exposedFunctions) {
			lua_pushlightuserdata(interpreter, parentObject);
			lua_pushcclosure(interpreter, iv.second, 1);
			lua_setfield(interpreter, -2, iv.first);
		}

	}

	lua_pushlightuserdata(interpreter, parentObject);
	lua_setfield(interpreter, -2, "cptr");
	lua_setfield(interpreter, -2, "cobj");
	//Set component reference tables
	const std::vector<Component*>* components = parentObject->getComponentsConst();
	for (auto component : *components)
	{
		const type_info& componentInfo = typeid(*component);
		
		map<const char*, lua_CFunction>* componentExposedFunctinos = GetExposedFunctionsForType(componentInfo);
		if (componentExposedFunctinos) {
			lua_newtable(interpreter);
			for (pair<const char*, lua_CFunction> iv : *componentExposedFunctinos) {
				lua_pushlightuserdata(interpreter, component);
				lua_pushcclosure(interpreter, iv.second, 1);
				lua_setfield(interpreter, -2, iv.first);
			}
		
			const char* componentName = strchr(componentInfo.name(), ' ') + 1;
			DUMP
			lua_setfield(interpreter, -2, componentName);
		}
	}

	_reference = luaL_ref(interpreter, LUA_REGISTRYINDEX);
}

void ScriptComponent::init() {

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
	//TODO: Push all of these to a global 'engine' table. So we can have something like (from lua): engine.test()
	lua_register(_luaInterpreter, "translate", &luaTranslate);
	lua_register(_luaInterpreter, "rotateAround", &luaRotateAround);
	lua_register(_luaInterpreter, "rotate", &luaRotate);
	lua_register(_luaInterpreter, "queryKeyDown", &luaQueryKeyDown);
	lua_register(_luaInterpreter, "getPosition", &luaGetPosition);
	lua_register(_luaInterpreter, "setPosition", &luaSetPosition);
	lua_register(_luaInterpreter, "luaSample", &luaSample);
	lua_register(_luaInterpreter, "draw", &luadDraw);
}

//TODO: Clean this up and properly test it. It's bound to have some leaks
int LuaManager::luaParseComponent(Script *script) {
	int res = luaL_loadbufferx(_luaInterpreter, script->_contents, script->_size, script->_name, nullptr);
	if (!lua_isfunction(_luaInterpreter, -1))
	{
		printf("[Lua]: Parsing failed for script {%s}: %s", script->_name, lua_tostring(_luaInterpreter, -1));
		return LUA_NOREF;
	}
	lua_pcall(_luaInterpreter, 0, 0, 0);

	lua_getglobal(_luaInterpreter, "currentClass");
	const char *className = lua_tostring(_luaInterpreter, -1);

	lua_getglobal(_luaInterpreter, className);
	if (lua_istable(_luaInterpreter, -1)) {

		lua_getglobal(_luaInterpreter, "extends");
		const char *extends = lua_tostring(_luaInterpreter, -1);
		if ((extends) && (strlen(extends) > 0)) {

			lua_pop(_luaInterpreter, 1);

			lua_getglobal(_luaInterpreter, extends);

			if (!lua_istable(_luaInterpreter, -1)) {
				printf("[Lua]: Parent class not loaded: {%s}, attempting to load...\n", extends);
				char path[64];
				strcpy(path, "/scripts/");
				strcat(path, extends);
				strcat(path, ".lua");
				//TODO: Break mutual dependancies between lua manager and asset manager
				ASSET_MANAGER->loadAsset<ScriptClass>(path);
				lua_pop(_luaInterpreter, 1);
				lua_getglobal(_luaInterpreter, extends);
				if (!lua_istable(_luaInterpreter, -1)) {
					printf("\nLua Parsing Error: lua script not found: {%s}!\n", path);
					lua_pop(_luaInterpreter, 1);
					return LUA_NOREF;
				}
			}

			//maybe lua rotate trick?
			lua_pushnil(_luaInterpreter);
			while (lua_next(_luaInterpreter, -2)) {
				const char* field = lua_tostring(_luaInterpreter, -2);
				int x = lua_getfield(_luaInterpreter, -5, field);
				if (x == LUA_TNIL) {
					lua_pop(_luaInterpreter, 1);
					lua_setfield(_luaInterpreter, -5, field);
				}
				else {
					lua_pop(_luaInterpreter, 2);
				}
			}

			lua_pop(_luaInterpreter, 2);
		}
		else {
			lua_pop(_luaInterpreter, 1);
		}
		const char * x1 = lua_tostring(_luaInterpreter, -1);
		//script->reference = luaL_ref(_luaInterpreter, LUA_REGISTRYINDEX);
		printf("[Lua]: Successfully loaded: {%s}\n", className);
		return luaL_ref(_luaInterpreter, LUA_REGISTRYINDEX);
	}
#if defined(_DEBUG)
	else {
		printf("[Lua]: Parsing Error: class not found: {%s}!", className);
		return LUA_NOREF;
	}
#endif
}

void LuaManager::luaParsePlainScript(Script* script)
{
}

void LuaManager::scriptCall(ScriptComponent * component, const char* name, float* params, int paramsNum) {
	if (!component) {
		return;
	}

	lua_rawgeti(_luaInterpreter, LUA_REGISTRYINDEX, component->getReference());

	lua_getfield(_luaInterpreter, -1, name);

	if (!lua_isfunction(_luaInterpreter, -1))
	{
		//printf("[Lua]: The lua function {%s} not defined", name);
	}
	else {
		for (int i = 0; i < paramsNum; i++) {
			lua_pushnumber(_luaInterpreter, params[i]);
		}
		lua_rawgeti(_luaInterpreter, LUA_REGISTRYINDEX, component->getReference());
		lua_insert(_luaInterpreter, -2);
		lua_call(_luaInterpreter, paramsNum + 1, 0);
		lua_pop(_luaInterpreter, paramsNum);
	}
}

int LuaManager::luaTranslate(lua_State *state) {
	GameObject* targetObject = static_cast<GameObject*>(lua_touserdata(state, -4));
	float x = lua_tonumber(state, -3);
	float y = lua_tonumber(state, -2);
	float z = lua_tonumber(state, -1);
	targetObject->_transform.translate(glm::vec3(x, y, z));
	return 0;
}

int LuaManager::luaRotateAround(lua_State* state) {
	GameObject* targetObject = static_cast<GameObject*>(lua_touserdata(state, -5));
	float x = lua_tonumber(state, -4);
	float y = lua_tonumber(state, -3);
	float z = lua_tonumber(state, -2);
	float amount = lua_tonumber(state, -1);
	targetObject->_transform.rotateAround(glm::vec3(x, y, z), amount);
	return 0;
}

int LuaManager::luaRotate(lua_State* state) {
	GameObject* targetObject = static_cast<GameObject*>(lua_touserdata(state, -4));
	float x = lua_tonumber(state, -3);
	float y = lua_tonumber(state, -2);
	float z = lua_tonumber(state, -1);
	targetObject->_transform.rotate(glm::vec3(x, y, z));
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

enum LuaDrawType {
	dPoint = 1,
	dLine,
};

int LuaManager::luadDraw(lua_State* state) {
	int type = lua_tonumber(state, -1);
	glColor3f(0.f, 0.f, 0.f);
	switch (type) {
		case dPoint: {
			float x = lua_tonumber(state, -4);
			float y	= lua_tonumber(state, -3);
			float z	= lua_tonumber(state, -2);
			glPointSize(5.f);
			glBegin(GL_POINTS);
			glVertex3f(x, y, z);
			glEnd();
			glFlush();
		} break;
	}
	return 0;
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
