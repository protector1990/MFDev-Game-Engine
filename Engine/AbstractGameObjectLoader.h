/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _ABSTRACT_GAME_OBJECT_LOADER_H
#define _ABSTRACT_GAME_OBJECT_LOADER_H

#include "GameObject.h"
#include <rapidxml-1.13\rapidxml.hpp>

class AbstractGameObjectLoader {
public:
	virtual GameObject* load(rapidxml::xml_node<char>* configuration) = 0;
protected:
	//Utility functions for all game objects
	glm::vec3 loadPosition(rapidxml::xml_node<char>* configuration);
	std::vector<ScriptComponent*> loadLuaComponents(rapidxml::xml_node<char>* configuration);
	//TODO: Videti ako moze nekako bez ovoga
	GameObject* _currentlyLoadingObject;
};
#endif