/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "GameObject.h"

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

GameObject* GameObject::getRoot() {
	if (_parent) {
		return _parent->getRoot();
	}
	return this;
}

GameObject* GameObject::getParent() {
	return _parent;
}

GameObject* GameObject::getChildren(int &count) {
	count = _children.size();
	return _children[0];
}

ScriptComponent* GameObject::getLuaComponent(const char* name) {
	for (int i = 0; i < _luaComponents.size(); i++) {
		if (strcmp(name, _luaComponents[i]->getScript()->_name) == 0) {
			return _luaComponents[i];
		}
	}
	return nullptr;
}

void GameObject::keyPressed(int keyPressed){
	//printf("Pressed: %i", keyPressed);
}