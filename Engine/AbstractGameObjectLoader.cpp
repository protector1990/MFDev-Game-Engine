/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "AbstractGameObjectLoader.h"
#include "AbstractComponentLoader.h"
#include "LoaderFactories.h"
#include "Common.h"

using namespace rapidxml;
using namespace std;

ComponentLoaderFactory componentLoaderFactory;

vector<ScriptComponent*> AbstractGameObjectLoader::loadScriptComponents(xml_node<char>* configuration) {
	vector<ScriptComponent*> ret;
	xml_node<char>* node = configuration->first_node();
	while (node) {
		char* x = node->name();
		if (strcmp(node->name(), "luaScript") == 0) {
			ScriptClass* luaClass = ASSET_MANAGER->loadAsset<ScriptClass>(node->value());
			if (luaClass) {
				ret.insert(ret.end(), new ScriptComponent(luaClass, _currentlyLoadingObject));
			}
#ifdef _DEBUG
			else {
				printf("%s%s", "Error loading lua script at ", node->value());
			}
#endif
		}
		node = node->next_sibling();
	}
	return ret;
}

vector<Component*> AbstractGameObjectLoader::loadComponents(xml_node<char>* configuration) {
	vector<Component*> ret;
	xml_node<char>* node = configuration->first_node();
	while (node) {
		char* x = node->name();
		if (strcmp(node->name(), "component") == 0) {
			char* type = node->first_attribute("type")->value();
			AbstractComponentLoader* loader = componentLoaderFactory.getComponentLoader(type);
			ret.push_back(loader->loadComponent(node, _currentlyLoadingObject));
		}
		node = node->next_sibling();
	}
	return ret;
}
