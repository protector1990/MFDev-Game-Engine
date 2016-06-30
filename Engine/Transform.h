/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _Transform_H
#define _Transform_H

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <SDL.h>

class Transform {
public:
	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec4 getRotation();
	void addPosition(glm::vec3 amount);
protected:
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::vec4 _rotation;
};
#endif