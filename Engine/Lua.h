/**  Copyright 2016 MarFil Studios  **/

#ifndef _LUA_SCRIPT_H
#define _LUA_SCRIPT_H

#include "..\libs\lua-5.3.2\src\lua.h"
#include "..\libs\lua-5.3.2\src\lualib.h"
#include "..\libs\lua-5.3.2\src\lauxlib.h"

class Script {
public:
	Script(const char *name, const char *contents, int size) :
		_name(name),
		_contents(contents),
		_size(size)
	{}
	~Script() {
		delete[] _contents;
		delete[] _name;
	}
	const char *_contents;
	const char *_name;
	const int _size;
};

class ScriptComponent {
public:
	ScriptComponent(Script *script, int reference) :
		_script(script),
		_reference(reference)
	{}

	Script* getScript();
	int getReference();

private:
	Script *_script;
	int _reference;
};

// TOOO: Create a new parent class ScriptManager, to make things more generic and separate script implementation from engine logic
// That would enabe easier implementation of another scripting language if needed in the future
// Remove the interpreter param from function calls. There will be always a single lua_state within the engine, no need to pass that around
// Also, it would make LuaManager take responsibility for lua_state and the rest of the egnine won't know about it at all.
class LuaManager {

	void luaBind(lua_State *interpreter);
	void luaExecute(lua_State *interpreter, Script *script);
	int luaQueryInput(lua_State*);
	void luaCall(lua_State* interpreter, Script *script, const char* name, int* params, int paramsNum);
	ScriptComponent* luaNewObject(lua_State *interpreter, Script *script);
	void luaLoad(lua_State *interpreter, Script *Script);

	static int luaTest(lua_State*);
	static int luaGameObjectUpdate(lua_State*);
	static int luaPrint(lua_State *state);
	static int luaSetPosition(lua_State *state);

};

#endif