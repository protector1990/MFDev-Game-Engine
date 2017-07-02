/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include <GL/glew.h>
#include "Sprite.h"
#include <stdexcept>
#include "Common.h"

using namespace glm;

//void Sprite::load(const char* cfgPath) {
//
//}
void Sprite::init() {
	// call lua init script from here
	//for (int i = 0; i < _scripts.size(); i++) {
	//	LuaManager::luaParseComponent(ENGINE.getLuaInterpreter(), _scripts[i]);
	//}

	//glGenBuffers(1, &_glVertexBufferObjects);
	//glGenVertexArrays(1, &_glVertexArray);
	//Optimize so that a single texture is generated exactly once and accessed from all the places it is required
	//glActiveTexture(GL_TEXTURE0);

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

void Sprite::render(SpriteRenderer *renderer) {
	//glColor3f(1.f, 1.f, 1.f);
	//GLenum i;
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

	//renderer->addQuads(_points, 4);
	//glBindVertexArray(_glVertexArray);
	//glBindBuffer(GL_ARRAY_BUFFER, _glVertexBufferObjects);
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, _texture->_glTexture);
	//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vec3), _points, GL_DYNAMIC_DRAW);

	//glColor4f(0.f, 1.f, 1.f, 1.f);

	//GLfloat texCoordinates[8] = { 0, 0, 1, 0, 1, 1, 0, 1 };
	//glBegin(GL_QUADS);
	//for (int i = 0; i < 4; i++) {
	//	glTexCoord2f(1,1);
	//	glVertex4f(_points[0].x, _points[0].y, _points[0].z, _points[0].w);
	//	//(-50.f, -50.f, 0.f);
	//	glTexCoord2f(0, 1);
	//	glVertex4f(_points[1].x, _points[1].y, _points[1].z, _points[1].w);
	//	//glVertex3f(50.f, -50.f, 0.f);
	//	glTexCoord2f(0, 0);
	//	glVertex4f(_points[2].x, _points[2].y, _points[2].z, _points[2].w);
	//	//glVertex3f(50.f, 50.f, 0.f);
	//	glTexCoord2f(1, 0);
	//	glVertex4f(_points[3].x, _points[3].y, _points[3].z, _points[3].w);
	//	//glVertex3f(-50.f, 50.f, 0.f);
	////}
	//glEnd();
	//
	////glDrawElements(GL_QUADS, 1, GL_UNSIGNED_INT, indices);
	////glBindVertexArray(0);

	//glDisable(GL_TEXTURE_2D);

	//i = glGetError();
}

const MTexture* Sprite::getTexture() {
	return _texture;
}
