#include "AbstractGameObjectLoader.h"
#include "Common.h"

using namespace rapidxml;

std::vector<ScriptComponent*> AbstractGameObjectLoader::loadLuaComponents(xml_node<char>* configuration) {
	std::vector<Script*> ret;
	//
	xml_node<char>* node = configuration->first_node();
	while (node) {
		char* x = node->name();
		if (strcmp(node->name(), "luaScript") == 0) {
			Script* script = ASSET_MANAGER->loadAsset<Script>(node->value());
			if (script) {
				ret.insert(ret.end(), script);
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