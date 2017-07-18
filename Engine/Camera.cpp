/**  Copyright 2017 MarFil Studios. All rights reserved.  **/
#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include "Common.h"

using namespace glm;

Camera::Camera() {
	VIDEO->addCamera(this);
}

Camera::~Camera() {
	VIDEO->removeCamera(this);
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
	glMultMatrixf(reinterpret_cast<float*>(&_projectionMatrix));
}

void Camera::postRender() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Camera::setOrtho(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
	_projectionMatrix = ortho(xMin, xMax, yMin, yMax, zMin, zMax); //Implement this by hand, too, if there is time
}

void Camera::setPerspective(float near, float far, float horizontalViewAngle, float verticalViewAngle) {
	float tanA = tan(glm::radians(horizontalViewAngle) / 2.f);
	float tanB = verticalViewAngle == 0.f ? ((float)_viewport.w / _viewport.z) * tanA : tan(glm::radians(verticalViewAngle) / 2.f);
	_projectionMatrix = mat4(0.f);
	_projectionMatrix[0][0] = 1.f / tanA;
	_projectionMatrix[1][1] = 1.f / tanB;
	_projectionMatrix[2][2] = (near + far) / (near - far);
	_projectionMatrix[3][2] = 2.f * near * far / (near - far);
	_projectionMatrix[2][3] = -1.f;
}

void Camera::setTakesWholeScreen(bool value) {
	_takesWholeScreen = value;
	if (value) {
		_viewport = vec4(0.f, 0.f, VIDEO->getWindowSize());
	}
}

void Camera::update(float deltaTime) {}
void Camera::init(){}
void Camera::render(){}