/** Copyright 2016 MarFil Studios **/

#include "GameObject.h"

#include <vector>

// Make this an abstract class too. Possible child classes:
// -Open World Scene
// -Interface Scene
// -Grand Strategy Scene
// -Level Scene
class Scene {
	//friend class Engine;
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
	virtual void update(float deltaTime);
	// Render scene
	void render(); 
	bool getActive();
protected:
	bool _isActive;
	std::vector<GameObject> _gameObjects;
};