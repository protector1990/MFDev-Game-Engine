/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Scene.h"
#include "Common.h"
#include "Lua.h"

GameObject* Scene::makeNewObject() {
	return NULL;
}
std::vector<GameObject*> Scene::getObjectsWithTags(int tags) {
	std::vector<GameObject*> a;
	return a;
}
// Load assets and instantiate game objects
Scene* Scene::load(const char *cfgPath, bool parallel) {
	return NULL;
}
bool Scene::unload(Scene *scene) {
	return false;
}
// Serialization and deserialization
bool Scene::serialize() {
	return false;
}
bool Scene::deserialize() {
	return false;
}
// Is updated and rendered?
void Scene::activate() {
	_isActive = true;
}
void Scene::deactivate(){

}
// Update scene
void Scene::update(float deltaTime) {
	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->update(deltaTime);
		//for (int j = 0; j < _gameObjects[i]->_scripts.size(); j++) {
			//luaExecute(ENGINE.getLuaInterpreter(), _gameObjects[i]->_scripts[j]);
		//}
	}
}
// Render scene
void Scene::render(Renderer *renderer) {
	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->render(renderer);
	}
}

bool Scene::getActive() {
	return _isActive;
}

void Scene::addGameObject(GameObject* gameObject) {
	_gameObjects.insert(_gameObjects.end(), gameObject);
}

void Scene::init() {
	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->init();
	}
}

void Scene::keyPressed(int keyPressed){
	for (unsigned int i = 0; i < _gameObjects.size(); i++){
		_gameObjects[i]->keyPressed(keyPressed);
	}
}