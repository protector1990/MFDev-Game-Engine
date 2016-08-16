/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "NavGridLoader.h"
#include "NavGrid.h"

using namespace rapidxml;
namespace temp_nav_grid_loader {
	char* transformID = "transform";
	char* positionXID = "positionX";
	char* positionYID = "positionY";
	char* positionZID = "positionZ";
}

GameObject* NavGridLoader::load(rapidxml::xml_node<char>* configuration) {
	NavGrid* ret = new NavGrid();
	// See where this line will fit best
	_currentlyLoadingObject = ret;

	xml_node<char>* transform = configuration->first_node(temp_nav_grid_loader::transformID);
	xml_node<char>* positionX = transform->first_node(temp_nav_grid_loader::positionXID);
	xml_node<char>* positionY = transform->first_node(temp_nav_grid_loader::positionYID);
	xml_node<char>* positionZ = transform->first_node(temp_nav_grid_loader::positionZID);
	ret->_Position.x = atof(positionX->value());
	ret->_Position.y = atof(positionY->value());
	ret->_Position.z = atof(positionZ->value());

	ret->_scriptComponents = loadScriptComponents(configuration);
	return (GameObject*)ret;
}
