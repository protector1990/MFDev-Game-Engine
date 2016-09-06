/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Transform.h"
//#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

vec3 Transform::getPosition() {
	vec4 raw = _transformations * vec4(0.f, 0.f, 0.f, 1.f);
	return vec3(raw.x / raw.w, raw.y / raw.w, raw.z / raw.w);
}

Transform* Transform::translate(vec3 amount) {
	mat4 translateMatrix =
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		amount.x, amount.y, amount.z, 1.f
	};
	_transformations *= translateMatrix;
	return this;
}

Transform* Transform::rotate(vec4 amount) {
	return this;
}

Transform* Transform::rotateAround(vec3 axis, float amount) {
	float c = cos(amount);
	float omc = 1.f - c;
	float s = sin(amount);
	float ax = axis.x;
	float ay = axis.y;
	float az = axis.z;

	mat4 rotateAroundMatrix =
	{
		c + omc * ax * ax, omc * ax * ay + s * az, omc * ax * az - s * ay, 0.f,
		omc * ax * ay - s * az, c + omc * ay * ay, omc * ay * az + s * ax, 0.f,
		omc * ax * az + s * ay, omc * ay * az - s * ax, c + omc * az * az, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	_transformations *= rotateAroundMatrix;
	return this;
}
