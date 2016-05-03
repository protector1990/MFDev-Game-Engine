/**  Copyright 2016 MarFil Studios  **/

#ifndef _Renderer_H
#define _Renderer_H

#include <glm\glm.hpp>
#include <vector>
#include <GL\glew.h>
#include <SDL_opengl.h>

class Renderer {
public:
	void init();
	void render(float deltaTime);
	void addTriangles(glm::vec3 *vertices, int length);
	void addQuads(glm::vec3 *vertices, int length);
private:
	// For now updated on each frame
	glm::vec3* _triangles;
	glm::vec3* _quads;
	int _trianglesVertexCount;
	int _quadsVertexCount;
	int _trianglesVertexCapacity = 50000;
	int _quadsVertexCapacity = 50000;
	GLuint _trianglesGLArray;
	GLuint _quadsGLArray;
};

#endif