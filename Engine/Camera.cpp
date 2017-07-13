/**  Copyright 2017 MarFil Studios. All rights reserved.  **/
#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include "Common.h"

using namespace glm;

Camera::Camera() {
	ENGINE.getVideo()->addCamera(this);
}

Camera::~Camera() {
	ENGINE.getVideo()->removeCamera(this);
}

void Camera::preRender() {
	// Set camera viewport
	if (_takesWholeScreen) {
		ivec2 windowsize = VIDEO->getWindowSize();
		glViewport(0, 0, windowsize.x, windowsize.y);
	}
	else {
		glViewport(_viewport.x, _viewport.y, _viewport.z, _viewport.w);
	}
	// Clear accoring to camera celaring settings
	if (_clearFlags & CLEAR_COLOR)
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	if (_clearFlags & CLEAR_DEPTH)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	// Push camera matrices
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	mat4 cameraTransformMatrix = _transform.getGlobalTransformationMatrix();
	glMultMatrixf(reinterpret_cast<float*>(&cameraTransformMatrix));
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(_xMin, _xMax, _yMin, _yMax, _zMin, _zMax);
}

void Camera::postRender() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Camera::update(float deltaTime) {}
void Camera::init(){}
void Camera::render(){}