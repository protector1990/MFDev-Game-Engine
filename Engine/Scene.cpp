/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Scene.h"
#include "Common.h"
#include "Lua.h"

GameObject* Scene::makeNewObject() {
	//Not Implemented
	return nullptr;
}
std::vector<GameObject*> Scene::getObjectsWithTags(unsigned tags) {
	std::vector<GameObject*> a;
	size_t objSize = _gameObjects.size();
	for (size_t i = 0; i < objSize; ++i)
	{
		if (_gameObjects[i]->compareTagAny(tags))
		{
			a.push_back(_gameObjects[i]);
		}
	}
	return a;
}

GameObject* Scene::getGameObjectByName(const char* name) const {
	for (GameObject* obj : _gameObjects)
	{
		if (!strcmp(name, obj->getName()))
		{
			return obj;
		}
	}
	return nullptr;
}

// Load assets and instantiate game objects
Scene* Scene::load(const char *cfgPath, bool parallel) {
	//Not Implemented
	return nullptr;
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
		_gameObjects[i]->updateComponents(deltaTime);
		//for (int j = 0; j < _gameObjects[i]->_scripts.size(); j++) {
			//luaExecute(ENGINE.getLuaInterpreter(), _gameObjects[i]->_scripts[j]);
		//}
	}
}
// Render scene
void Scene::render(SpriteRenderer *renderer) {
	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->render(renderer);
		_gameObjects[i]->renderDebugComponents(0.f);
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
	for (unsigned int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->keyPressed(keyPressed);
	}
}