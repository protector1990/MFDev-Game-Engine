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
