/** Copyright 2016 MarFil Studios. All rights reserved. **/

#pragma once

#include "Component.h"

class Animator : public Component {
public:
	Animator(GameObject* parentObject) : Component(parentObject) {}
	virtual void init() override;
	virtual void update(float deltaTime) override;

};