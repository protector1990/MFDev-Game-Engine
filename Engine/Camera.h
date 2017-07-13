/**  Copyright 2017 MarFil Studios. All rights reserved.  **/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"

#define CLEAR_COLOR 1
#define CLEAR_DEPTH 2
#define CLEAR_3 4
#define CLEAR_4 8
#define CLEAR_5 16
#define CLEAR_6 32
#define CLEAR_7 64
#define CLEAR_8 128

// TODO: Rethink seeing camera as a game object. It should have transform, however, it doesn't have to inherit GameObject to do so.
class Camera : public GameObject {
	friend class CameraLoader;
public:
	Camera();
	~Camera();
	inline void setProjectionMatrix(glm::mat4 projectionMatrix) { _projectionMatrix = projectionMatrix; }
	void preRender();
	void postRender();
	inline void setOrtho(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
		_xMin = xMin;
		_xMax = xMax;
		_yMin = yMin;
		_yMax = yMax;
		_zMin = zMin;
		_zMax = zMax;
	}
	inline void setViewport(glm::ivec4 viewport) { _viewport = viewport; }
	void update(float deltaTime) override;
	void init() override;
	void render() override;
	inline short getRenderingOrder() const { return _renderingOrder; }
	inline void setClearFlags(unsigned short clearFlags) { _clearFlags = clearFlags; }
protected:
	bool _takesWholeScreen = false;
	unsigned short _clearFlags = 3;
	glm::mat4 _projectionMatrix;
	short _renderingOrder;
	glm::ivec4 _viewport;
	float _xMin, _xMax, _yMin, _yMax, _zMin, _zMax;
	// TODO: implement cases when user tries to set rendering order from within the render loop
	inline void setRenderingOrder(short value) { _renderingOrder = value; }
};