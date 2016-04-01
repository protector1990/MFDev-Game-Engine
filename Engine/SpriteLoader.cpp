#include "SpriteLoader.h"
#include "Sprite.h"
#include "Engine.h"

using namespace rapidxml;

#define ASSET_MANAGER Engine::getInstance().getAssetManager()

char* textureID = "texture";
char* sheetID = "sheet";
char* widthID = "width";
char* heightID = "height";
char* animSpeedID = "animSpeed";

GameObject* SpriteLoader::load(xml_node<char>* configuration) {
	Sprite* ret = new Sprite();
	xml_node<char>* texture_node = configuration->first_node(textureID);
	char* texture_path = texture_node->value();
	ret->_texture = ASSET_MANAGER->loadAsset<SDL_Surface>(texture_path);
	xml_node<char>* sheet = configuration->first_node(sheetID);
	xml_node<char>* sheetWidth = sheet->first_node(widthID);
	xml_node<char>* sheetHeight = sheet->first_node(widthID);
	xml_node<char>* animSpeed = sheet->first_node(animSpeedID);
	ret->animSpeed = atof(animSpeed->value());
	ret->sheetHeight = atoi(sheetWidth->value());
	ret->sheetWidth = atoi(sheetHeight->value());
	return (GameObject*)ret;
}