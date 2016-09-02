/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <SDL.h>
#include <stack>

enum TransformReference {
	local,
	global
};

enum TransformComponent {
	position,
	scale,
	rotation
};

class Transform {
public:
	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec4 getRotation();  // Quaternion
	glm::vec3 getLocalPosition();
	glm::vec3 getLocalScale();
	glm::vec4 getLocalRotation();  // Quaternion
	void addPosition(glm::vec3 amount);
	glm::vec4 get(TransformReference pointOfReference, TransformComponent component);
protected:
	glm::vec3 _localPosition;
	glm::vec3 _localScale;
	glm::vec4 _localRotation;
};
#endif