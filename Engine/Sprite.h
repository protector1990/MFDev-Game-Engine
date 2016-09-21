/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _SPRITE_H
#define _SPRITE_H

#include "GameObject.h"
#include <SDL.h>
#include "Renderer.h"
#include <glm\glm.hpp>
#include <SDL_opengl.h>
#include "Texture.h"

class Sprite : public virtual GameObject {
	friend class SpriteLoader;
public:
	void init() override;
	void update(float deltaTime) override;
	void render(Renderer *renderer) override;
	const MTexture* getTexture();
protected:
	int _sheetWidth = 1;
	int _sheetHeight = 1;
	float _animSpeed = 1.f;
	// Rework this later so that points are always kept at constant values in local space
	// and maybe deformed by animations, and only transform matrixes given to renderer so that
	// the shader can move vertices aroyund in world space
	// This should be done for all of the vertices in all the classes
	glm::vec4 _points[4];

	//Opengl specifics. Move this somewhere else at some point
	GLuint _glVertexArray;
	GLuint _glVertexBufferObjects;
	MTexture* _texture;
};
#endif