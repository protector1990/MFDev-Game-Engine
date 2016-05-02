#ifndef _Sprite_H
#define _Sprite_H

#include "GameObject.h"
#include <SDL.h>
#include "Renderer.h"
#include <glm\glm.hpp>

class Sprite : public GameObject {
	friend class SpriteLoader;
public:
	// void load(const char* cfgPath) override;
	void init() override;
	void update(float deltaTime) override;
	void render(Renderer *renderer) override;
	const SDL_Surface* getTexture();
protected:
	SDL_Surface *_texture;
	int _sheetWidth = 1;
	int _sheetHeight = 1;
	float _animSpeed = 1.f;
	// Rework this later so that points are always kept at constant values in local space
	// and maybe deformed by animations, and only transform matrixes given to renderer so that
	// the shader can move vertices aroyund in world space
	// This should be done for all of the vertices in all the classes
	glm::vec3 _points[4];

	GLuint _vertexArray;
	GLuint _vertexBufferObjects;
};
#endif