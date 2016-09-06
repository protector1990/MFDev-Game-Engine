/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Common.h"
#include "SpriteLoader.h"
#include "Sprite.h"
#include "Engine.h"
#include <vector>

using namespace rapidxml;

//TODO: fix, fix, fix
namespace temp_sprite_loader {

	char* transformID = "transform";
	char* positionXID = "positionX";
	char* positionYID = "positionY";
	char* positionZID = "positionZ";

	char* textureID = "texture";
	char* sheetID = "sheet";
	char* widthID = "width";
	char* heightID = "height";
	char* animSpeedID = "animSpeed";
}

GameObject* SpriteLoader::load(xml_node<char>* configuration) {
	Sprite* ret = new Sprite();

	const char* tempName = configuration->first_attribute("name")->value();
	ret->_name = new char[strlen(tempName) + 1];
	strcpy(ret->_name, tempName);

	// See where this line will fit best
	_currentlyLoadingObject = ret;


	xml_node<char>* texture_node = configuration->first_node(temp_sprite_loader::textureID);
	char* texture_path = texture_node->value();
	ret->_texture = ASSET_MANAGER->loadAsset<SDL_Surface>(texture_path);

	xml_node<char>* transform = configuration->first_node(temp_sprite_loader::transformID);
	xml_node<char>* positionX = transform->first_node(temp_sprite_loader::positionXID);
	xml_node<char>* positionY = transform->first_node(temp_sprite_loader::positionYID);
	xml_node<char>* positionZ = transform->first_node(temp_sprite_loader::positionZID);
	ret->_transform.translate(glm::vec3(atof(positionX->value()), atof(positionY->value()), atof(positionZ->value())));
	//ret->_Position.x = atof(positionX->value());
	//ret->_Position.y = atof(positionY->value());
	//ret->_Position.z = atof(positionZ->value());

	xml_node<char>* sheet = configuration->first_node(temp_sprite_loader::sheetID);
	xml_node<char>* sheetWidth = sheet->first_node(temp_sprite_loader::widthID);
	xml_node<char>* sheetHeight = sheet->first_node(temp_sprite_loader::widthID);
	xml_node<char>* animSpeed = sheet->first_node(temp_sprite_loader::animSpeedID);
	ret->_animSpeed = atof(animSpeed->value());
	ret->_sheetHeight = atoi(sheetWidth->value());
	ret->_sheetWidth = atoi(sheetHeight->value());
	ret->_components = loadComponents(configuration);
	ret->_scriptComponents = loadScriptComponents(configuration);

	return static_cast<GameObject*>(ret);
}