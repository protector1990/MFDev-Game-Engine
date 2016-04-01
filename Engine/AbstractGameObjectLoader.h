#ifndef _AbstractGameObjectLoader_H
#define _AbstractGameObjectLoader_H

#include "GameObject.h"
#include <rapidxml-1.13\rapidxml.hpp>

class AbstractGameObjectLoader {
public:
	virtual GameObject* load(rapidxml::xml_node<char>* configuration) = 0;
};
#endif