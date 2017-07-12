/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include <GL/glew.h>
#include "Sprite.h"
#include <stdexcept>
#include "Common.h"

using namespace glm;

void Sprite::init() {
}

void Sprite::update(float deltaTime) {

}

GLuint indices[] = { 0, 1, 2, 3 };

mat4 pointOffsetMatrix = mat4
{
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
};

vec4 tmp(0.f, 0.f, 0.f, 1.f);

void Sprite::render() {
	SpriteRenderer *renderer = ENGINE.getVideo()->_spriteRenderer;
	//TODO: See if it is practical to offload this to a vertex shader
	mat4 masterTransformationMatrix = _transform.getGlobalTransformationMatrix();
	
	pointOffsetMatrix[3][0] = -_texture->_nativeTexture->w / 2.f;
	pointOffsetMatrix[3][1] = -_texture->_nativeTexture->h / 2.f;

	// if we ever get multithreading for whatever reason, make sure that each thread has its own copy of this matrix
	_points[0] = masterTransformationMatrix * pointOffsetMatrix * tmp;
	pointOffsetMatrix[3][0] *= -1.f;
	_points[1] = masterTransformationMatrix * pointOffsetMatrix * tmp;
	pointOffsetMatrix[3][1] *= -1.f;
	_points[2] = masterTransformationMatrix * pointOffsetMatrix * tmp;
	pointOffsetMatrix[3][0] *= -1.f;
	_points[3] = masterTransformationMatrix * pointOffsetMatrix * tmp;

	// Budzi, hahahahaha, budzi
	vec3 noHomoPoints[4] = {
		vec3(_points[0].x / _points[0].w, _points[0].y / _points[0].w, _points[0].z / _points[0].w),
		vec3(_points[1].x / _points[1].w, _points[1].y / _points[1].w, _points[1].z / _points[1].w),
		vec3(_points[2].x / _points[2].w, _points[2].y / _points[2].w, _points[2].z / _points[2].w),
		vec3(_points[3].x / _points[3].w, _points[3].y / _points[3].w, _points[3].z / _points[3].w)
	};

	renderer->addQuads(noHomoPoints, 4, _texture, _materials.size() > 0 ? _materials[0] : nullptr);
}

const MTexture* Sprite::getTexture() {
	return _texture;
}
