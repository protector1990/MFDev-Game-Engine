/** Copyright 2016 MarFil Studios **/

#ifndef _Scene_H
#define _Scene_H

#include "GameObject.h"

#include <vector>
#include "Renderer.h"

// Make this an abstract class too. Possible child classes:
// -Open World Scene
// -Interface Scene
// -Grand Strategy Scene
// -Level Scene
class Scene {
	//friend class Engine;
	// Refactor this to function with getters and setters instead of friend classes
	friend class WinAssetManager;
public:
	GameObject* makeNewObject();
	std::vector<GameObject*> getObjectsWithTags(int tags);
	// Load assets and instantiate game objects
	virtual Scene* load(const char *cfgPath, bool parallel);
	virtual bool unload(Scene *scene);
	// Serialization and deserialization
	bool serialize();
	bool deserialize();
	// Is updated and rendered?
	void activate();
	void deactivate();
	// Update scene
	// TODO: make a separate updateInternal function
	virtual void update(float deltaTime);
	// Render scene
	void render(Renderer *renderer); 
	bool getActive();
	void init();
	void keyPressed(int keyPressed);

protected:
	bool _isActive;
	std::vector<GameObject*> _gameObjects;
	char* _name;
	void addGameObject(GameObject* gameObject);
};

#endif