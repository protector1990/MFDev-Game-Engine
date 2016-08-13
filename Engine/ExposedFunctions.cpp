/** Copyright 2016 MarFil Studios. All rights reserved. **/
// This file should be auto generated in the future

#include "ExposedFunctions.h"
#include "NavGrid.h"
#include <map>
#include "Common.h"

using namespace std;

map<const char*, int> MappedValues;

int LuaNavGridSetSampleDensity(lua_State* state) {
	NavGrid* targetObject = static_cast<NavGrid*>(lua_touserdata(state, -2));
	//TODO: make error checking
	targetObject->setSampleDensity(lua_tonumber(state, -1));
	return 0;
}

void ExposeFunctionsToScript() {
	lua_State* interpreter = SCRIPT_MANAGER->getLuaInterpreter();
	// First, create a table with C functions for this type
	lua_createtable(interpreter, 0, 1);
	// Now add functions
	lua_pushcfunction(interpreter, LuaNavGridSetSampleDensity);
	lua_setfield(interpreter, -2, "setSampleDensity");
	lua_pop(interpreter, 1);
	// Reference the table and return it
	MappedValues.insert(make_pair(typeid(NavGrid).name(), luaL_ref(interpreter, -1)));
}

int GetExposedFunctionsForType(const type_info &type) {
	map<const char*, int>::iterator it = MappedValues.find(type.name());
	if (it == MappedValues.end())
	{
		return 0;
	}
	return MappedValues.at(type.name());
}