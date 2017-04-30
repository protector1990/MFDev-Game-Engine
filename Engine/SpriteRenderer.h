/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include <assimp-3.2/include/assimp/material.h>
#include "Material.h"

#define MAX_QUADS_SIZE 30
#define MAX_QUADS_VERTICES 120

class SpriteRenderer : public Renderer {
public:
	void init() override;
	void render(float deltaTime);
	void addTriangles(glm::vec3 *vertices, int length);
	void addQuads(glm::vec3 *vertices, int length, MTexture* texture, Material* customMaterial = nullptr);
	void preRender() override;
	void postRender() override;
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
	Material* _currentMaterial;
	glm::vec2* _spriteTexCoordArray;
	MTexture* _currentlyActiveTexture;
};