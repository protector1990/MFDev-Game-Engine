/** Copyright 2016 MarFil Studios. All rights reserved. **/

#pragma once
#include "Component.h"
#include <rapidxml-1.13/rapidxml.hpp>

class AbstractComponentLoader
{
public:
	virtual Component* loadComponent(rapidxml::xml_node<char>* configuration, GameObject* parentObject) = 0;
};

