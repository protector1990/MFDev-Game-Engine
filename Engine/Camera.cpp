/**  Copyright 2017 MarFil Studios. All rights reserved.  **/
#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>

using namespace glm;

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
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
}

void Camera::postRender() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}