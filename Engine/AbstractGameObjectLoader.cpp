/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "AbstractGameObjectLoader.h"
#include "Common.h"

using namespace rapidxml;

std::vector<ScriptComponent*> AbstractGameObjectLoader::loadLuaComponents(xml_node<char>* configuration) {
	std::vector<ScriptComponent*> ret;
	xml_node<char>* node = configuration->first_node();
	while (node) {
		char* x = node->name();
		if (strcmp(node->name(), "luaScript") == 0) {
			ScriptClass* luaClass = ASSET_MANAGER->loadAsset<ScriptClass>(node->value());
			if (luaClass) {
				ret.insert(ret.end(), new ScriptComponent(luaClass, _currentlyLoadingObject));
			}
#ifdef DEBUG
			else {
				printf("%s%s", "Error loading lua script at ", node->value());
			}
#endif
		}
		node = node->next_sibling();
	}
	return ret;
}