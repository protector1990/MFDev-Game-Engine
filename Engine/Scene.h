/** Copyright 2016 MarFil Studios **/

#include <vector>
#include "GameObejct.h"

// Make this an abstract class too. Possible child classes:
// -Open World Scene
// -Interface Scene
// -Grand Strategy Scene
// -Level Scene
class Scene {
public:
	GameObject* makeNewObject();
	std::vector<GameObject*> getObjectsWithTags(int tags);
	// Load assets and instantiate game objects
	static Scene* load(const char *cfgPath, bool parallel);
	static bool unload(Scene *scene);
	// Serialization and deserialization
	bool serialize();
	bool deserialize();
	// Is updated and rendered?
	void activate();
	void deactivate();
	// Update scene
	void update(float deltaTime);
	// Render scene
	void render(); 
private:
	bool _isActive;
	std::vector<GameObject> _gameObjects;
};