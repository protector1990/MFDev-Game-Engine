#include "Sprite.h"
#include <GL\glew.h>
#include <GL\GL.h>

using namespace glm;

//void Sprite::load(const char* cfgPath) {
//
//}
void Sprite::init() {
	// empty for now
	// call lua init script from here
	glGenBuffers(1, &_vertexBufferObjects);
	glGenVertexArrays(sizeof(vec3), &_vertexArray);
	
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
	glBindVertexArray(_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vec3), _points, GL_DYNAMIC_DRAW);
	glBindTextures()

}

const SDL_Surface* Sprite::getTexture() {
	return _texture;
}
