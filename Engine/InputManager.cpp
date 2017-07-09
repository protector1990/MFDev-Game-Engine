/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#include "InputManager.h"

using namespace std;

bool InputManager::queryKeyDown(char key) {
	if (_keysDown.find(key) != _keysDown.end()) {
		return true;
	}
	return false;
}

bool InputManager::queryMousePressed(char key) {
	return _mouseDown;
}

glm::vec2 InputManager::queryMousePosition() {
	return _mousePosition;
}

void InputManager::keyPressed(char key) {
	//printf("Key pressed: %c %i\n", key, key);
	_keysDown.insert(key);
}

void InputManager::keyReleased(char key) {
	_keysDown.erase(_keysDown.find(key));
}

void InputManager::mousePressed(char key) {
	_mouseDown = true;
}

void InputManager::mouseReleased(char key) {
	_mouseDown = false;
}

void InputManager::changeMousePos(glm::vec2 position) {
	_mousePosition = position;
}
