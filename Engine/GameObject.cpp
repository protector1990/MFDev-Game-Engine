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

ScriptComponent* GameObject::getLuaComponent(const char* name) {
	for (int i = 0; i < _luaComponents.size(); i++) {
		if (strcmp(name, _luaComponents[i]->getScript()->_name) == 0) {
			return _luaComponents[i];
		}
	}
}

void GameObject::keyPressed(int keyPressed){
	//printf("Pressed: %i", keyPressed);
}