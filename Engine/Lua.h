/**  Copyright 2016 MarFil Studios  **/

#include "..\libs\lua-5.3.2\src\lua.h"
#include "..\libs\lua-5.3.2\src\lualib.h"
#include "..\libs\lua-5.3.2\src\lauxlib.h"

class LuaScript {
public:
	LuaScript(const char *name, const char *contents, int size) :
		_name(name),
		_contents(contents),
		_size(size)
	{}
	~LuaScript() {
		delete[] _contents;
		delete[] _name;
	}
	const char *_contents;
	const char *_name;
	const int _size;
};

void luaBind(lua_State *interpreter);

void luaExecute(lua_State *interpreter, LuaScript *script);

int luaTest(lua_State*);