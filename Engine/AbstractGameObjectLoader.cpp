/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "AbstractGameObjectLoader.h"
#include "AbstractComponentLoader.h"
#include "LoaderFactories.h"
#include "Common.h"

using namespace rapidxml;
using namespace glm;
using namespace std;

ComponentLoaderFactory componentLoaderFactory;

namespace AbsGameObjLoaderTextConstants {
	char* transformID = "transform";
	char* positionXID = "positionX";
	char* positionYID = "positionY";
	char* positionZID = "positionZ";

	char* scaleXID = "scaleX";
	char* scaleYID = "scaleY";
	char* scaleZID = "scaleZ";

	char* rotationXID = "rotationX";
	char* rotationYID = "rotationY";
	char* rotationZID = "rotationZ";


	char* parent = "parent";
}

void AbstractGameObjectLoader::commonLoad(xml_node<char>* configuration) {
	const char* tempName = configuration->first_attribute("name")->value();
	_currentlyLoadingObject->_name = new char[strlen(tempName) + 1];
	strcpy(_currentlyLoadingObject->_name, tempName);

	xml_node<char>* transform = configuration->first_node(AbsGameObjLoaderTextConstants::transformID);
	xml_node<char>* positionX = transform->first_node(AbsGameObjLoaderTextConstants::positionXID);
	xml_node<char>* positionY = transform->first_node(AbsGameObjLoaderTextConstants::positionYID);
	xml_node<char>* positionZ = transform->first_node(AbsGameObjLoaderTextConstants::positionZID);

	xml_node<char>* scaleX = transform->first_node(AbsGameObjLoaderTextConstants::scaleXID);
	xml_node<char>* scaleY = transform->first_node(AbsGameObjLoaderTextConstants::scaleYID);
	xml_node<char>* scaleZ = transform->first_node(AbsGameObjLoaderTextConstants::scaleZID);

	xml_node<char>* rotationX = transform->first_node(AbsGameObjLoaderTextConstants::rotationXID);
	xml_node<char>* rotationY = transform->first_node(AbsGameObjLoaderTextConstants::rotationYID);
	xml_node<char>* rotationZ = transform->first_node(AbsGameObjLoaderTextConstants::rotationZID);
	_currentlyLoadingObject->_transform.translate(vec3(
		positionX ? atof(positionX->value()) : 0.f,
		positionY ? atof(positionY->value()) : 0.f,
		positionZ ? atof(positionZ->value()) : 0.f
	));
	_currentlyLoadingObject->_transform.rotate(vec3(
		rotationX ? atof(rotationX->value()) : 0.f,
		rotationY ? atof(rotationY->value()) : 0.f,
		rotationZ ? atof(rotationZ->value()) : 0.f
	));
	_currentlyLoadingObject->_transform.setScale(vec3(
		scaleX ? atof(scaleX->value()) : 1.f,
		scaleY ? atof(scaleY->value()) : 1.f,
		scaleZ ? atof(scaleZ->value()) : 1.f
	));
	xml_node<char>* parent = transform->first_node(AbsGameObjLoaderTextConstants::parent);
	if (parent)
	{
		_currentlyLoadingObject->setParent(_currentlyLoadingScene->getGameObjectByName(parent->value()));
	}
	_currentlyLoadingObject->_scriptComponents = loadScriptComponents(configuration);
}

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
