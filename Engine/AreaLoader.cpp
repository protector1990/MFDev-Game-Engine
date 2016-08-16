/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "AreaLoader.h"
#include "Area.h"

using namespace rapidxml;
using namespace glm;

//TODO: Add error checking, checking if points are on the same plane
Component* AreaLoader::loadComponent(xml_node<char>* configuration, GameObject* parentObject) {
	Area* ret = new Area(parentObject);
	for (xml_node<char>* vertex = configuration->first_node("vertex"); vertex; vertex = vertex->next_sibling("vertex"))
	{
		ret->addPoint(vec3(atof(vertex->first_attribute("x")->value()), atof(vertex->first_attribute("y")->value()), 
			atof(vertex->first_attribute("z")->value())));
	}
	return ret;
}
