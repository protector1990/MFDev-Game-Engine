/** Copyright 2016 MarFil Studios **/

#ifndef _GameObject_H
#define _GameObject_H

#include <vector>
#include "Renderer.h"

// Abstract game object class. Should be implemented as needed for specific game 
// requirements. You can go as far as programming every single game object
// as separate child class, or have a more generic game object implementation
// For now Unity componented style objects are performed, although every new object type must be
// defined as a c++ class. That enables game objects to have some predefined behaviours (differs from
// Unity, where complete GameObject behaviour is defined through its components
class GameObject {
public:
	//GameObject();
	unsigned int getTag();
	void setTag(unsigned int tag);
	//virtual void load(const char* cfgPath) = 0;
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render(Renderer *renderer) = 0;

	virtual GameObject *getRoot();
	virtual GameObject *getParent();
	virtual GameObject *getChildren(int &count);

	template<class T> T* getChild();

	glm::vec3 _Position;

protected:
	unsigned int _tag;
	GameObject *_parent;
	std::vector<GameObject*> _children;
};
#endif