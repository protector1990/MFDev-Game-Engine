/**  Copyright 2017 MarFil Studios. All rights reserved.  **/
#pragma once

#include <glm/glm.hpp>
#include "GameObject.h"

#define CLEAR_COLOR 1
#define CLEAR_DEPTH 2

class Camera : GameObject {
public:
	void setProjectionMatrix(glm::mat4 projectionMatrix);
	void preRender();
	void postRender();
protected:
	unsigned short _clearFlags = 0;
	glm::mat4 _projectionMatrix;
	short _renderingOrder;
};