/** Copyright 2016 MarFil Studios. All rights reserved. **/
// This file should be auto generated in the future

#include "ExposedFunctions.h"
#include "NavGrid.h"
#include "Sprite.h"
#include <map>
//#include <list>
#include "Common.h"
#include "Area.h"
#include "Camera.h"

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

int AreaContains(lua_State* state)
{
	Component* intermediate = static_cast<Component*>(lua_touserdata(state, lua_upvalueindex(1)));
	Area* targetArea = dynamic_cast<Area*>(intermediate);
	bool contains = targetArea->contains(vec3(lua_tonumber(state, -3), lua_tonumber(state, -2), lua_tonumber(state, -1)));
	lua_pushboolean(state, contains);
	return 1;
}

int AddUniformFloat(lua_State* state)
{
	GameObject* gameObject = static_cast<GameObject*>(lua_touserdata(state, lua_upvalueindex(1)));
	float uniformValue = lua_tonumber(state, -1);
	const char* uniformName = lua_tostring(state, -2);
	int index = lua_tointeger(state, -3);
	Material* material = gameObject->getMaterial(index);
	if (!material)
	{
		return 0;
	}
	gameObject->getMaterial(0)->pushUniformFloat(uniformName, uniformValue);
	return 0;
}

int AddUniformVec3(lua_State* state)
{
	GameObject* gameObject = static_cast<GameObject*>(lua_touserdata(state, lua_upvalueindex(1)));
	float uniformValueX = lua_tonumber(state, -3);
	float uniformValueY = lua_tonumber(state, -2);
	float uniformValueZ = lua_tonumber(state, -1);
	const char* uniformName = lua_tostring(state, -4);
	int index = lua_tointeger(state, -5);
	Material* material = gameObject->getMaterial(index);
	if (!material)
	{
		return 0;
	}
	material->pushUniformVec3(uniformName, vec3(uniformValueX, uniformValueY, uniformValueZ));
	return 0;
}

int AddUniformVec4(lua_State* state)
{
	GameObject* gameObject = static_cast<GameObject*>(lua_touserdata(state, lua_upvalueindex(1)));
	float uniformValueX = lua_tonumber(state, -4);
	float uniformValueY = lua_tonumber(state, -3);
	float uniformValueZ = lua_tonumber(state, -2);
	float uniformValueW = lua_tonumber(state, -1);
	const char* uniformName = lua_tostring(state, -5);
	int index = lua_tointeger(state, -6);
	Material* material = gameObject->getMaterial(index);
	if (!material)
	{
		return 0;
	}
	material->pushUniformVec4(uniformName, vec4(uniformValueX, uniformValueY, uniformValueZ, uniformValueW));
	return 0;
}

int AddUniformMat4(lua_State* state)
{
	//TODO: implement later
	GameObject* gameObject = static_cast<GameObject*>(lua_touserdata(state, lua_upvalueindex(1)));
	return 0;
}

int AddUniformSampler2D(lua_State* state)
{
	GameObject* gameObject = static_cast<GameObject*>(lua_touserdata(state, lua_upvalueindex(1)));
	GLint uniformValue = lua_tointeger(state, -1);
	const char* uniformName = lua_tostring(state, -2);
	int index = lua_tointeger(state, -3);
	Material* material = gameObject->getMaterial(index);
	if (!material)
	{
		return 0;
	}
	material->pushUniformSampler2D(uniformName, uniformValue);
	return 0;
}

int SetPerspective(lua_State* state)
{
	GameObject* gameObject = static_cast<GameObject*>(lua_touserdata(state, lua_upvalueindex(1)));
	Camera* camera = dynamic_cast<Camera*>(gameObject);
	float verticalAngle = lua_tonumber(state, -1);
	float horizontalAngle = lua_tonumber(state, -2);
	float far = lua_tonumber(state, -3);
	float near = lua_tonumber(state, -4);
	camera->setPerspective(near, far, horizontalAngle, verticalAngle);
	return 0;
}

void ExposeFunctionsToScript()
{
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

	map<const char*, lua_CFunction>* cameraFuns = new map<const char*, lua_CFunction>;
	cameraFuns->insert(make_pair("SetPerspective", SetPerspective));
	MappedValues.insert(make_pair(typeid(Camera).name(), cameraFuns));

	map<const char*, lua_CFunction>* materialFuns = new map<const char*, lua_CFunction>;
	materialFuns->insert(make_pair("AddUniformFloat", AddUniformFloat));
	materialFuns->insert(make_pair("AddUniformVec3", AddUniformVec3));
	materialFuns->insert(make_pair("AddUniformVec4", AddUniformVec4));
	materialFuns->insert(make_pair("AddUniformMat4", AddUniformMat4));
	MappedValues.insert(make_pair(typeid(GameObject).name(), materialFuns));
}

map<const char*, lua_CFunction>* GetExposedFunctionsForType(const type_info &type)
{
	map<const char*, map<const char*, lua_CFunction>*>::iterator it = MappedValues.find(type.name());
	if (it == MappedValues.end())
	{
		return nullptr;
	}
	if (typeid(GameObject).before(type))
	{
		// join
		auto ret = new map<const char*, lua_CFunction>();
		auto derivedTypeExposedFuns = MappedValues.at(type.name());
		auto gameObjectExposedFuns = MappedValues.at(typeid(GameObject).name());

		for (auto const& pair : *derivedTypeExposedFuns)
		{
			ret->insert(pair);
		}

		for (auto const& pair : *gameObjectExposedFuns)
		{
			ret->insert(pair);
		}

		return ret; // Fix this, optimize and remove memory leaks
	}
	else
	{
		return MappedValues.at(type.name());
	}
}