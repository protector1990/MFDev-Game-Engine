/** Copyright 2016 MarFil Studios. All rights reserved. **/
// This file should be auto generated in the future

#include "ExposedFunctions.h"
#include "NavGrid.h"



void ExposeFunctionsToScript() {
	
}

int LuaNavGridSetSampleDensity(lua_State* state) {
	NavGrid* targetObject = static_cast<NavGrid*>(lua_touserdata(state, -2));
	targetObject->setSampleDensity(lua_tonumber(state, -1));
	return 0;
}