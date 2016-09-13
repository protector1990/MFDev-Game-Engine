/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm/detail/type_mat.hpp>

class GameObject;

enum TransformReference {
	local,
	global
};

enum TransformComponent {
	position,
	scale,
	rotation
};

struct CoordinateSystem {
	glm::vec4 x;
	glm::vec4 y;
	glm::vec4 z;
};

class Transform {
public:
	Transform(GameObject* host) : _gameObject(host) {}
	glm::vec3 getPosition() const;
	glm::vec3 getScale();
	glm::vec4 getRotation();  // Quaternion
	glm::vec3 getLocalPosition();
	glm::vec3 getLocalScale();
	glm::vec4 getLocalRotation();  // Quaternion
	Transform* translate(glm::vec3 amount); // Builder type trick
	Transform* rotate(glm::vec4 amount);
	Transform* rotateAround(glm::vec3 axis, float amount);
	Transform* rotateX(float amount);
	Transform* rotateY(float amount);
	Transform* rotateZ(float amount);
	Transform* rotate(glm::vec3 amounts);
	Transform* scale(glm::vec3 amount);
	Transform* globalScale(glm::vec3 amount);
	glm::vec3 worldToLocalCoordinates(glm::vec3& point);
	glm::vec3 localToWorldCoordinates(glm::vec3& point);
	glm::vec4 get(TransformReference pointOfReference, TransformComponent component); //Usage example: get(local, scale)
	glm::vec4 get(TransformComponent component, Transform* relativeTo);
	glm::mat4 getLocalTransformationMatrix() const;
	glm::mat4 getGlobalTransformationMatrix() const;
protected:
	glm::vec3 _localPosition;
	glm::vec3 _localScale;
	glm::vec4 _localRotation;
	glm::mat4 _transformations;
	glm::mat4 _scaleStack;
	GameObject* _gameObject;
	glm::mat4 getGlobalTransformationMatrixInverseScale() const;
	CoordinateSystem getCoordinateSystem();
};
#endif