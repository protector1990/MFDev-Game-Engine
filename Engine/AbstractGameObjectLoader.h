#ifndef _AbstractGameObjectLoader_H
#define _AbstractGameObjectLoader_H

#include "GameObject.h"
#include <rapidxml-1.13\rapidxml.hpp>

class AbstractGameObjectLoader {
public:
	virtual GameObject* load(rapidxml::xml_node<char>* configuration) = 0;
protected:
	//Utility functions for all game objects
	glm::vec3 loadPosition(rapidxml::xml_node<char>* configuration);
	std::vector<LuaScript*> loadLuaScripts(rapidxml::xml_node<char>* configuration);
};
#endif