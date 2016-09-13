/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Transform.h"
#include "GameObject.h"
#include <stack>
//#include <iostream>

using namespace glm;
using namespace std;

vec3 Transform::getPosition() const {
	// Still would keep this and not use getGlobalTransformationMatrix because this should be more efficient
	stack<const mat4> parentMatrixStack;
	mat4 transformMatrix = mat4
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	GameObject* parent = _gameObject->_parent;
	while (parent)
	{
		parentMatrixStack.push(parent->_transform.getLocalTransformationMatrix());
		parent = parent->_parent;
	}

	while (parentMatrixStack.size() > 0)
	{
		transformMatrix *= parentMatrixStack.top();
		parentMatrixStack.pop();
	}

	transformMatrix *= _transformations * _scaleStack;
	
	vec4 raw = transformMatrix * vec4(0.f, 0.f, 0.f, 1.f);
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

//TODO: Check all of these matrices

Transform* Transform::rotateX(float amount) {
	//TODO: Check the efficiency of GLM trigonometric functions
	float c = cos(amount);
	float s = sin(amount);

	mat4 rotateMatrix = 
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, c, s, 0.f,
		0.f, -s, c, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	
	_transformations *= rotateMatrix;
	return this;
}

Transform* Transform::rotateY(float amount) {
	float c = cos(amount);
	float s = sin(amount);

	mat4 rotateMatrix =
	{
		c, 0.f, -s, 0.f,
		0.f, 1.f, 0.f, 0.f,
		s, 0.f, c, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	_transformations *= rotateMatrix;
	return this;
}

Transform* Transform::rotateZ(float amount) {
	float c = cos(amount);
	float s = sin(amount);

	mat4 rotateMatrix =
	{
		c, s, 0.f, 0.f,
		-s, c, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	_transformations *= rotateMatrix;
	return this;
}

Transform* Transform::rotate(vec3 amounts) {
	// TODO: See how we can avoid calling updateCoordinateSystem 3 times in single rotate() call
	rotateX(amounts.x);
	rotateY(amounts.y);
	rotateZ(amounts.z);
	return this;
}

Transform* Transform::scale(glm::vec3 amount) {
	mat4 scaleMatrix =
	{
		amount.x, 0.f, 0.f, 0.f,
		0.f, amount.y, 0.f, 0.f,
		0.f, 0.f, amount.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	_scaleStack *= scaleMatrix;
	return this;
}

Transform* Transform::globalScale(glm::vec3 amount) {
	mat4 scaleMatrix =
	{
		amount.x, 0.f, 0.f, 0.f,
		0.f, amount.y, 0.f, 0.f,
		0.f, 0.f, amount.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	printf("ScalingX: %f\n", amount.x);
	_transformations = scaleMatrix * _transformations;
	return this;
}

glm::vec3 Transform::worldToLocalCoordinates(vec3& point) {
	//vec3 position = getPosition();
	vec3 delta = point - getPosition();
	vec4 delta4(delta.x, delta.y, delta.z, 1.f);
	CoordinateSystem localSystem = getCoordinateSystem();
	//std::cout << "engine coord system:\nX: " << localSystem.x.x << " " << localSystem.x.y << " " << localSystem.x.z \
		<< "\nY: " << localSystem.y.x << " " << localSystem.y.y << " " << localSystem.y.z \
		<< "\nZ: " << localSystem.z.x << " " << localSystem.z.y << " " << localSystem.z.z << "\n";
	return vec3(dot(delta4, localSystem.x), dot(delta4, localSystem.y), dot(delta4, localSystem.z));
}

glm::vec3 Transform::localToWorldCoordinates(vec3& point) {
	return vec3(0, 0, 0);
}

glm::mat4 Transform::getLocalTransformationMatrix() const {
	return _transformations * _scaleStack;
}

//Gets global transformation matrix. Scaling included
glm::mat4 Transform::getGlobalTransformationMatrix() const {
	if (_gameObject->_parent)
	{
		return _gameObject->_parent->_transform.getGlobalTransformationMatrix() * _transformations * _scaleStack;
	}
	return _transformations * _scaleStack;
}

glm::mat4 Transform::getGlobalTransformationMatrixInverseScale() const {
	if (_gameObject->_parent)
	{
		return _gameObject->_parent->_transform.getGlobalTransformationMatrix() * _transformations * inverse(_scaleStack);
	}
	return _transformations * inverse(_scaleStack);
}

CoordinateSystem Transform::getCoordinateSystem() {
	CoordinateSystem ret;
	mat4 globalTransformationMatrix = getGlobalTransformationMatrixInverseScale();
	ret.x = globalTransformationMatrix * vec4(1.f, 0.f, 0.f, 0.f);
	ret.y = globalTransformationMatrix * vec4(0.f, 1.f, 0.f, 0.f);
	ret.z = globalTransformationMatrix * vec4(0.f, 0.f, 1.f, 0.f);
	return ret;
}
