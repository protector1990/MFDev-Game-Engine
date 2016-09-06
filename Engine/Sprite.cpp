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
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_glTexture);

	glBindTexture(GL_TEXTURE_2D, _glTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//TODO: read color mode from _texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _texture->w, _texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, _texture->pixels);
}

void Sprite::update(float deltaTime) {

}

GLuint indices[] = { 0, 1, 2, 3 };

void Sprite::render(Renderer *renderer) {
	// This code is still very experimental
	glColor3f(1.f, 1.f, 1.f);
	GLenum i;
	vec3 position = _transform.getPosition();
	_points[0].x = _points[3].x = position.x - _texture->w / 2.f;
	_points[1].x = _points[2].x = position.x + _texture->w / 2.f;
	_points[0].y = _points[1].y = position.y - _texture->h / 2.f;
	_points[2].y = _points[3].y = position.y + _texture->h / 2.f;
	_points[0].z = _points[1].z = _points[2].z = _points[3].z = position.z;
	//renderer->addQuads(_points, 4);
	//glBindVertexArray(_glVertexArray);
	//glBindBuffer(GL_ARRAY_BUFFER, _glVertexBufferObjects);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _glTexture);
	//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vec3), _points, GL_DYNAMIC_DRAW);

	//glColor4f(0.f, 1.f, 1.f, 1.f);

	GLfloat texCoordinates[8] = { 0, 0, 1, 0, 1, 1, 0, 1 };
	glBegin(GL_QUADS);
	//for (int i = 0; i < 4; i++) {
		glTexCoord2f(1,1);
		glVertex3f(_points[0].x, _points[0].y, _points[0].z);
		//(-50.f, -50.f, 0.f);
		glTexCoord2f(0, 1);
		glVertex3f(_points[1].x, _points[1].y, _points[1].z);
		//glVertex3f(50.f, -50.f, 0.f);
		glTexCoord2f(0, 0);
		glVertex3f(_points[2].x, _points[2].y, _points[2].z);
		//glVertex3f(50.f, 50.f, 0.f);
		glTexCoord2f(1, 0);
		glVertex3f(_points[3].x, _points[3].y, _points[3].z);
		//glVertex3f(-50.f, 50.f, 0.f);
	//}
	glEnd();
	
	//glDrawElements(GL_QUADS, 1, GL_UNSIGNED_INT, indices);
	//glBindVertexArray(0);

	glDisable(GL_TEXTURE_2D);

	i = glGetError();
}

const SDL_Surface* Sprite::getTexture() {
	return _texture;
}
