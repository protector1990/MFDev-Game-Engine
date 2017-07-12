/**  Copyright 2017 MarFil Studios. All rights reserved.  **/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"

#define CLEAR_COLOR 1
#define CLEAR_DEPTH 2

class Camera : public GameObject {
public:
	Camera();
	~Camera();
	inline void setProjectionMatrix(glm::mat4 projectionMatrix) { _projectionMatrix = projectionMatrix; }
	void preRender();
	void postRender();
	inline void setOrtho(float xMin, float xMax, float yMin, float yMax, float zMin = -10, float zMax = 10) {
		_xMin = xMin;
		_xMax = xMax;
		_yMin = yMin;
		_yMax = yMax;
		_zMin = zMin;
		_zMax = zMax;
		glm::lookAt();
	}
	inline void setViewport(glm::ivec4 viewport) { _viewport = viewport; }
protected:
	unsigned short _clearFlags = 3;
	glm::mat4 _projectionMatrix;
	short _renderingOrder;
	glm::ivec4 _viewport;
	float _xMin, _xMax, _yMin, _yMax, _zMin, _zMax;
};