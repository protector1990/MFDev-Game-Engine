/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "GameObject.h"
#include "Common.h"
#include "Component.h"

GameObject::~GameObject() {
}

unsigned GameObject::getTag() const {
	return _tag;
}

void GameObject::setTag(unsigned tag) {
	_tag = tag;
}

bool GameObject::compareTag(unsigned tag) const {
	// Redesign this. Currently, works by asking if object contains any of the tags.
	// Maybe introduce both && and || logic
	if (_tag & tag)
	{
		return true;
	}
	return false;
}

void GameObject::appendTag(unsigned tag) {
	_tag |= tag;
}

void GameObject::updateComponents(float deltaTime) {
	for (unsigned int i = 0; i < _scriptComponents.size(); i++) {
		SCRIPT_MANAGER->scriptCall(_scriptComponents[i], "update", &deltaTime, 1);
	}
}

#ifdef _DEBUG
void GameObject::renderDebugComponents(float deltaTime) {
	for (size_t i = 0; i < _components.size(); i++)
	{
		_components[i]->renderDebug(deltaTime);
	}
	for (size_t i = 0; i < _scriptComponents.size(); i++)
	{
		SCRIPT_MANAGER->scriptCall(_scriptComponents[i], "draw", &deltaTime, 1);
	}
}
#endif // _DEBUG


GameObject* GameObject::getRoot() {
	if (_parent) {
		return _parent->getRoot();
	}
	return this;
}

GameObject* GameObject::getParent() {
	return _parent;
}

void GameObject::setParent(GameObject* newParent) {
	//TODO: add logic for calculating relative position to new parent
	_parent = newParent;
}

GameObject* GameObject::getChildren(int &count) {
	count = _children.size();
	return _children[0];
}

ScriptComponent* GameObject::getLuaComponent(const char* name) {
	for (int i = 0; i < _scriptComponents.size(); i++) {
		if (strcmp(name, _scriptComponents[i]->getScriptClass()->getScript()->_name) == 0) {
			return _scriptComponents[i];
		}
	}
	return nullptr;
}

void GameObject::keyPressed(int keyPressed) {
	//printf("Pressed: %i\n", keyPressed);
}

const ScriptableInterface* GameObject::getScriptableIterface() const {
	return &_scriptableInterface;
}

const std::vector<Component*>* GameObject::getComponentsConst() const {
	return &_components;
}

const char* GameObject::getName() const {
	return _name;
}
