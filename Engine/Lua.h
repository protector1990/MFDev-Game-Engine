/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

//TODO: Make all of these generic and scripting language independant

#ifndef _LUA_SCRIPT_H
#define _LUA_SCRIPT_H

#include <lua-5.3.2/src/lua.h>
#include <lua-5.3.2/src/lualib.h>
#include <lua-5.3.2/src/lauxlib.h>

#include "TextAsset.h"

class GameObject;

class Script : public TextAsset {
public:
	Script(const char *name, const char *contents, int size) :
		TextAsset(name, contents, size)
	{}
};

class ScriptClass {
public:
	Script* getScript() const;

	void setScript(Script* script);

	int getReference() const;

	void setReference(int reference);

protected:
	Script *script;
	int _reference;
};

class ScriptComponent {
public:
	ScriptComponent(ScriptClass *script, GameObject *parentObject);

	ScriptClass* getScriptClass() const;
	int getReference() const;

private:
	ScriptClass *_script;
	int _reference;
};

// TOOO: Create a new parent class ScriptManager, to make things more generic and separate script implementation from engine logic
// That would enabe easier implementation of another scripting language if needed in the future
// Remove the interpreter param from function calls. There will be always a single lua_state within the engine, no need to pass that around
// Also, it would make LuaManager take responsibility for lua_state and the rest of the egnine won't know about it at all.
class LuaManager {
public:
	void luaBind() const;
	int luaQueryInput(lua_State*);
	static void luaCall(lua_State* interpreter, ScriptComponent *component, const char* name, float* params, int paramsNum);
	static int luaParseComponent(lua_State *interpreter, Script *script);
	static void luaParsePlainScript(lua_State *interpreter, Script *script);
	void luaCopyTable(int index);
	void luaLoad(lua_State *interpreter, Script *Script);

	void initManager();

	static int luaTest(lua_State*);
	static int luaGameObjectUpdate(lua_State*);
	static int luaPrint(lua_State *state);
	static int luaTranslate(lua_State *state);
	static int luaGetPosition(lua_State *state);
	static int luaSetPosition(lua_State *state);
	static int luaQueryKeyDown(lua_State *state);
	lua_State* getLuaInterpreter() const;
private:
	lua_State* 	_luaInterpreter;
	int _gameObjectMetaTable;
};

#endif