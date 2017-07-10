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

	transformMatrix *= _translationMatrix * _rotationMatrix * _scaleStack;
	
	vec4 raw = transformMatrix * vec4(0.f, 0.f, 0.f, 1.f);
	return vec3(raw.x / raw.w, raw.y / raw.w, raw.z / raw.w);
}

glm::vec3 Transform::getLocalScale() const {
	//TODO: implement this without matrix/vector multiplication. Should take up less cycles
	vec4 rawLocalScale = _scaleStack * vec4(1.f, 1.f, 1.f, 1.f);
	return vec3(rawLocalScale.x / rawLocalScale.w, rawLocalScale.y / rawLocalScale.w, rawLocalScale.z / rawLocalScale.w);
}

vec3 Transform::getLocalRotation() const {
	vec4 rawRotation = _miscRotations * vec4(0.f, 0.f, 0.f, 1.f);
	return vec3(rawRotation.x / rawRotation.w, rawRotation.y / rawRotation.w, rawRotation.z / rawRotation.w);
}

Transform* Transform::translate(vec3 amount) {
	mat4 currentTranslationMatrix =
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		amount.x, amount.y, amount.z, 1.f
	};
	_translationMatrix *= currentTranslationMatrix;
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

	_rotationMatrix *= rotateAroundMatrix;
	_miscRotations *= rotateAroundMatrix;
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
	
	_rotationMatrix *= rotateMatrix;
	_miscRotations *= rotateMatrix;
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

	_rotationMatrix *= rotateMatrix;
	_miscRotations *= rotateMatrix;
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

	_rotationMatrix *= rotateMatrix;
	_miscRotations *= rotateMatrix;
	return this;
}

Transform* Transform::rotate(vec3 amounts) {
	float cx = cos(amounts.x);
	float sx = sin(amounts.x);

	float cy = cos(amounts.y);
	float sy = sin(amounts.y);

	float cz = cos(amounts.z);
	float sz = sin(amounts.z);

	mat4 rotateXMatrix =
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, cx, sx, 0.f,
		0.f, -sx, cx, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	mat4 rotateYMatrix =
	{
		cy, 0.f, -sy, 0.f,
		0.f, 1.f, 0.f, 0.f,
		sy, 0.f, cy, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	mat4 rotateZMatrix = {
		cz, sz, 0.f, 0.f,
		-sz, cz, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	mat4 combinedRotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

	_rotationMatrix *= combinedRotateMatrix;
	_miscRotations *= combinedRotateMatrix;
	return this;
}

Transform* Transform::setScale(glm::vec3 amount) {
	_scaleStack = 
	{
		amount.x, 0.f, 0.f, 0.f,
		0.f, amount.y, 0.f, 0.f,
		0.f, 0.f, amount.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	return this;
}

Transform* Transform::setPosition(glm::vec3 position) {
	// TODO: implement keeping separate transformation matrices for translation, rotation and scale
	_translationMatrix = 
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		position.x, position.y, position.z, 1.f
	};
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
	_scaleStack = scaleMatrix * _scaleStack;
	return this;
}

glm::vec3 Transform::worldToLocalCoordinates(vec3& point) {
	vec3 delta = point - getPosition();
	vec4 delta4(delta.x, delta.y, delta.z, 1.f);
	CoordinateSystem localSystem = getInverseScaleCoordinateSystem();
	return vec3(dot(delta4, localSystem.x), dot(delta4, localSystem.y), dot(delta4, localSystem.z));
}

glm::vec3 Transform::localToWorldCoordinates(vec3& point) {
	//TODO: Possible optimization for getting coordinate systems is to have coordinate system as a class member, \
	  and then make private updateCoordinateSystem that would get the reference to the member coordinate system. \
	  Method getInverseScaleCoordinateSystem() should become public and for external use
	CoordinateSystem localSystem = getCoordinateSystem();
	//See if there is a more optimal solution
	vec4 globalOrientationPoint = point.x *  localSystem.x + point.y * localSystem.y + point.z * localSystem.z;
	vec3 globalOrientationPoint3 = vec3(globalOrientationPoint.x,
		globalOrientationPoint.y,
		globalOrientationPoint.z);
	return globalOrientationPoint3 + getPosition();
}

glm::mat4 Transform::getLocalTransformationMatrix() const {
	return _translationMatrix * _rotationMatrix * _scaleStack;
}

//Gets global transformation matrix. Scaling included
glm::mat4 Transform::getGlobalTransformationMatrix() const {
	if (_gameObject->_parent)
	{
		return _gameObject->_parent->_transform.getGlobalTransformationMatrix() * _translationMatrix * _rotationMatrix * _scaleStack;
	}
	return _translationMatrix * _rotationMatrix * _scaleStack;
}

glm::mat4 Transform::getGlobalTransformationMatrixInverseScale() const {
	if (_gameObject->_parent)
	{
		return _gameObject->_parent->_transform.getGlobalTransformationMatrix() * _translationMatrix * _rotationMatrix * inverse(_scaleStack);
	}
	return _translationMatrix * _rotationMatrix * inverse(_scaleStack);
}

// Budz, budz
CoordinateSystem Transform::getInverseScaleCoordinateSystem() {
	CoordinateSystem ret;
	mat4 globalTransformationMatrix = getGlobalTransformationMatrixInverseScale();
	ret.x = globalTransformationMatrix * vec4(1.f, 0.f, 0.f, 0.f);
	ret.y = globalTransformationMatrix * vec4(0.f, 1.f, 0.f, 0.f);
	ret.z = globalTransformationMatrix * vec4(0.f, 0.f, 1.f, 0.f);
	return ret;
}

CoordinateSystem Transform::getCoordinateSystem() {
	CoordinateSystem ret;
	mat4 globalTransformationMatrix = getGlobalTransformationMatrix();
	ret.x = globalTransformationMatrix * vec4(1.f, 0.f, 0.f, 0.f);
	ret.y = globalTransformationMatrix * vec4(0.f, 1.f, 0.f, 0.f);
	ret.z = globalTransformationMatrix * vec4(0.f, 0.f, 1.f, 0.f);
	return ret;
}
