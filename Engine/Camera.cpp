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
	mat4 cameraTransformMatrix = _transform.getGlobalTransformationMatrix();
	glMultMatrixf(reinterpret_cast<float*>(&cameraTransformMatrix));
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
}

void Camera::postRender() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}