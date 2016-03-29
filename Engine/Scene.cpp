#include "Scene.h"

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

}
void Scene::deactivate(){

}
// Update scene
void Scene::update(float deltaTime) {

}
// Render scene
void Scene::render() {

}

bool Scene::getActive() {
	return false;
}