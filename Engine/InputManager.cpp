/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#include "InputManager.h"

using namespace std;

bool InputManager::queryKeyDown(char key) {
	if (_keysDown.find(key) != _keysDown.end()) {
		return true;
	}
	return false;
}

void InputManager::keyPressed(char key) {
	//printf("Key pressed: %c %i\n", key, key);
	_keysDown.insert(key);
}

void InputManager::keyReleased(char key) {
	_keysDown.erase(_keysDown.find(key));
}