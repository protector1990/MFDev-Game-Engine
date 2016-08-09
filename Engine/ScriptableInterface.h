/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _SCRIPTABLE_INTERFACE_H
#define _SCRIPTABLE_INTERFACE_H
#include <lua-5.3.2/src/lua.h>
#include <map>

//TODO: Make more generic. Game Objects should know nothing about LUA
class ScriptableInterface {
public:
	const std::map<char*, lua_CFunction>* getFunctions;
	lua_CFunction operator[](char*) const;
	void push_back(char*, lua_CFunction);
protected:
	std::map<char*, lua_CFunction> _exposedFuncions;
};


#endif  // #ifndef _SCRIPTABLE_INTERFACE_H