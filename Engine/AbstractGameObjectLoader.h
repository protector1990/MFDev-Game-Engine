/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _ABSTRACT_GAME_OBJECT_LOADER_H
#define _ABSTRACT_GAME_OBJECT_LOADER_H

#include "GameObject.h"
#include <rapidxml-1.13\rapidxml.hpp>

//TODO: Fix object and component loaders hierarchy. Define what should go where and make teh system robust
class AbstractGameObjectLoader {
public:
	// Lock load with mutex in implementations
	virtual GameObject* load(rapidxml::xml_node<char>* configuration) = 0;
protected:
	//Utility functions for all game objects
	glm::vec3 loadPosition(rapidxml::xml_node<char>* configuration);
	std::vector<ScriptComponent*> loadScriptComponents(rapidxml::xml_node<char>* configuration);
	std::vector<Component*> loadComponents(rapidxml::xml_node<char>* configuration);
	void commonLoad(rapidxml::xml_node<char>* configuration);
	//TODO: Videti ako moze nekako bez ovoga
	GameObject* _currentlyLoadingObject;
public:
	Scene* _currentlyLoadingScene;
};

#endif