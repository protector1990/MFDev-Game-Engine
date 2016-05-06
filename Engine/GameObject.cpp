/** Copyright 2016 MarFil Studios **/

#include "GameObject.h"

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

LuaScript* GameObject::getScript(const char* name) {
	for (int i = 0; i < _scripts.size(); i++) {
		if (strcmp(name, _scripts[i]->_name) == 0) {
			return _scripts[i];
		}
	}
}

void GameObject::keyPressed(int keyPressed){
	//printf("Pressed: %i", keyPressed);
}