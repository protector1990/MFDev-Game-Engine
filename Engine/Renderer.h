/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#ifndef _RENDERER_H
#define _RENDERER_H

#include <glm\glm.hpp>
#include <vector>
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Shader.h"
#include "Texture.h"

#define MAX_QUADS_SIZE 30
#define MAX_QUADS_VERTICES 120

class Renderer {
public:
	void init();
	void render(float deltaTime);
	void addTriangles(glm::vec3 *vertices, int length);
	void addQuads(glm::vec3 *vertices, int length, MTexture* texture);
private:
	// For now updated on each frame
	glm::vec3* _triangles;
	glm::vec3* _quads;
	int _trianglesVertexCount;
	int _quadsVertexCount;
	int _trianglesVertexCapacity = 50000;
	int _quadsVertexCapacity = MAX_QUADS_VERTICES;
	GLuint _trianglesGLArray;
	GLuint _quadsGLArray;
	void drawTriangles();
	void drawQuads();
protected:
	ShaderProgram* _spriteShaderProgram;
	glm::vec2* _spriteTexCoordArray;
};

#endif