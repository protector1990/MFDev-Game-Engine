/** Copyright 2016 MarFil Studios **/

#include "Common.h"
#include "SpriteLoader.h"
#include "Sprite.h"
#include "Engine.h"
#include <vector>

using namespace rapidxml;

char* textureID = "texture";
char* sheetID = "sheet";
char* widthID = "width";
char* heightID = "height";
char* animSpeedID = "animSpeed";
char* transformID = "transform";
char* positionXID = "positionX";
char* positionYID = "positionY";
char* positionZID = "positionZ";

GameObject* SpriteLoader::load(xml_node<char>* configuration) {
	Sprite* ret = new Sprite();
	xml_node<char>* texture_node = configuration->first_node(textureID);
	char* texture_path = texture_node->value();
	ret->_texture = ASSET_MANAGER->loadAsset<SDL_Surface>(texture_path);

	xml_node<char>* transform = configuration->first_node(transformID);
	xml_node<char>* positionX = transform->first_node(positionXID);
	xml_node<char>* positionY = transform->first_node(positionYID);
	xml_node<char>* positionZ = transform->first_node(positionZID);
	ret->_Position.x = atof(positionX->value());
	ret->_Position.y = atof(positionY->value());
	ret->_Position.z = atof(positionZ->value());

	xml_node<char>* sheet = configuration->first_node(sheetID);
	xml_node<char>* sheetWidth = sheet->first_node(widthID);
	xml_node<char>* sheetHeight = sheet->first_node(widthID);
	xml_node<char>* animSpeed = sheet->first_node(animSpeedID);
	ret->_animSpeed = atof(animSpeed->value());
	ret->_sheetHeight = atoi(sheetWidth->value());
	ret->_sheetWidth = atoi(sheetHeight->value());
	ret->_luaComponents = loadLuaComponents(configuration);
	return (GameObject*)ret;
}