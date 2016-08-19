/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "GameObject.h"

//Macro for defining what game object a component must be put on or what other components it requires to function properly
#define REQUIRE()

class Component {
public:
	Component(GameObject*);
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void renderDebug(float deltaTime){}
protected:
	GameObject* _parentObject;
};
#endif
