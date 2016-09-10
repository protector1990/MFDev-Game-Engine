/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#include "ConfigurationManager.h"
#include "Common.h"
using namespace rapidxml;

void ConfigurationManager::init() {
	_configFileRaw = ASSET_MANAGER->loadAsset<TextAsset>("_config");
	_configFile.parse<0>(const_cast<char*>(_configFileRaw->_contents));
}

const char* ConfigurationManager::getString(const char* id) const {
	// Not implemented
	return nullptr;
}

int ConfigurationManager::getInt(const char* id) const {
	// Not implemented
	return 0;
}

float ConfigurationManager::getFloat(const char* id) const {
	//Not implemented
	return 0.f;
}

const char* ConfigurationManager::getScenePath(const char* name) const {
	xml_node<char> *config = _configFile.first_node("config");
	xml_node<char> *scenes = config->first_node("scenes");
	for (xml_node<char> *scene = scenes->first_node("scene"); scene; scene = scene->next_sibling())
	{
		xml_attribute<char> *sceneName = scene->first_attribute("name");
		if (!sceneName) continue;
		if (strcmp(scene->name(), "scene") == 0 && sceneName && strcmp(name, sceneName->value()) == 0)
		{
			return scene->value();
		}
	}
	return nullptr;
}

const char* ConfigurationManager::getScenePath(int orderNum) const {
	xml_node<char> *config = _configFile.first_node("config");
	xml_node<char> *scenes = config->first_node("scenes");
	for (xml_node<char> *scene = scenes->first_node("scene"); scene; scene = scene->next_sibling())
	{
		xml_attribute<char> *sceneNumber = scene->first_attribute("orderNum");
		if (!sceneNumber) continue;
		int num = atoi(sceneNumber->name());
		if (strcmp(scene->name(), "scene") == 0 && sceneNumber && num == orderNum)
		{
			return scene->value();
		}
	}
	return nullptr;
}

const char* ConfigurationManager::getSCriptPath(const char* name) const {
	//Not implemented
	return nullptr;
}
