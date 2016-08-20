/** Copyright 2016 MarFil Studios. All rights reserved. **/
// This file should be auto generated in the future

#include "ExposedFunctions.h"
#include "NavGrid.h"
#include "Sprite.h"
#include <map>
//#include <list>
#include "Common.h"
#include "Area.h"

using namespace std;
using namespace glm;

map<const char*, map<const char*, lua_CFunction>*> MappedValues;

int LuaNavGridSetSampleDensity(lua_State* state) {
	GameObject* intermediate = static_cast<GameObject*>(lua_touserdata(state, lua_upvalueindex(1)));
	NavGrid* targetObject = dynamic_cast<NavGrid*>(intermediate);
	//TODO: make error checking
	targetObject->setSampleDensity(lua_tonumber(state, -1));
	return 0;
}

int SampleSpriteFun(lua_State* state) {
	GameObject* intermediate = static_cast<GameObject*>(lua_touserdata(state, lua_upvalueindex(1)));
	Sprite* targetObject = dynamic_cast<Sprite*>(intermediate);
	//targetObject->sampleFun();
	return 0;
}

int AreaContains(lua_State* state) {
	Component* intermediate = static_cast<Component*>(lua_touserdata(state, lua_upvalueindex(1)));
	Area* targetArea = dynamic_cast<Area*>(intermediate);
	bool contains = targetArea->contains(vec3(lua_tonumber(state, -3), lua_tonumber(state, -2), lua_tonumber(state, -1)));
	lua_pushboolean(state, contains);
	return 1;
}

void ExposeFunctionsToScript() {
	lua_State* interpreter = SCRIPT_MANAGER->getLuaInterpreter();

	map<const char*, lua_CFunction>* navGridFuns = new map<const char*, lua_CFunction>;
	navGridFuns->insert(make_pair("SetSampleDensity", LuaNavGridSetSampleDensity));
	MappedValues.insert(make_pair(typeid(NavGrid).name(), navGridFuns));

	map<const char*, lua_CFunction>* spriteFuns = new map<const char*, lua_CFunction>;
	spriteFuns->insert(make_pair("SampleSpriteFun", SampleSpriteFun));
	MappedValues.insert(make_pair(typeid(Sprite).name(), spriteFuns));

	map<const char*, lua_CFunction>* areaFuns = new map<const char*, lua_CFunction>;
	areaFuns->insert(make_pair("Contains", AreaContains));
	MappedValues.insert(make_pair(typeid(Area).name(), areaFuns));
}

map<const char*, lua_CFunction>* GetExposedFunctionsForType(const type_info &type) {
	map<const char*, map<const char*, lua_CFunction>*>::iterator it = MappedValues.find(type.name());
	if (it == MappedValues.end())
	{
		return 0;
	}
	return MappedValues.at(type.name());
}