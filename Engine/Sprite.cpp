#include <GL\glew.h>
#include "Sprite.h"
#include <stdexcept>

using namespace glm;

//void Sprite::load(const char* cfgPath) {
//
//}
void Sprite::init() {
	// empty for now
	// call lua init script from here
	glGenBuffers(1, &_glVertexBufferObjects);
	glGenVertexArrays(sizeof(vec3), &_glVertexArray);
	//Optimize so that a single texture is generated exactly once and accessed from all the places it is required
	glGenTextures(1, &_glTexture);

	glBindTexture(GL_TEXTURE_2D, _glTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	try {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _texture->w,
			_texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			_texture->pixels);
	}
	catch (...) {
		int x = 0;
	}
}

void Sprite::update(float deltaTime) {
	// empty for now
}

void Sprite::render(Renderer *renderer) {
	_points[0].x = _points[1].x = _Position.x - _texture->w / 2.f;
	_points[2].x = _points[3].x = _Position.x + _texture->w / 2.f;
	_points[0].y = _points[1].y = _Position.y - _texture->h / 2.f;
	_points[2].y = _points[3].y = _Position.y + _texture->h / 2.f;
	_points[0].z = _points[1].z = _points[2].z = _points[3].z = _Position.z;
	//renderer->addQuads(_points, 4);
	glBindVertexArray(_glVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, _glVertexBufferObjects);
	glBindTexture(GL_TEXTURE_2D, _glTexture);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vec3), _points, GL_DYNAMIC_DRAW);
	glDrawElements(GL_QUADS, 1, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

const SDL_Surface* Sprite::getTexture() {
	return _texture;
}
