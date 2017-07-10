/**  Copyright 2016 MarFil Studios. All rights reserved.  **/




#include "Common.h"
#include "Engine.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "3DAsset.h"
#include <..\include\assimp\scene.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Sprite.h"
#include <iostream>
#include <thread>
#include <algorithm>

using namespace std;
using namespace glm;

GLfloat texCoordinates[8] = { 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f };
vec2 texCoordinatesVec[4] = { vec2(1.f, 1.f), vec2(0.f, 1.f), vec2(0.f, 0.f), vec2(1.f, 0.f) };

//helper function
void LoadMedia() {
	//image = ASSET_MANAGER->loadAsset<SDL_Surface>("/textures/ZnamSDL.bmp");
	//model = ASSET_MANAGER->loadAsset<Asset3D>("/models/house.fbx");
	//meshModel = ASSET_MANAGER->loadAsset<Model3D>("/models/path.fbx");
	//meshModel->setUp();	
}

GLfloat ambient[] = { 0.5f, 0.5f, 0.5f };
GLfloat mat_specular[] = { 0.7, 0.2, 0.5, 1.0 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat light_position[] = { 10.0, 10.0, 10.0, 1.0 };

void SpriteRenderer::init() {

	GLint majVer;
	GLint minVer;

	glGetIntegerv(GL_MAJOR_VERSION, &majVer);
	glGetIntegerv(GL_MINOR_VERSION, &minVer);

	 //Sprite shader
	ShaderProgram spriteShader;
	spriteShader._frag = ASSET_MANAGER->loadAsset<Shader>("/shaders/FSpriteShader.glsl");
	spriteShader._vert = ASSET_MANAGER->loadAsset<Shader>("/shaders/VSpriteShader.glsl");
	spriteShader._id = glCreateProgram();
	glAttachShader(spriteShader._id, spriteShader._vert->_shaderObject);
	glAttachShader(spriteShader._id, spriteShader._frag->_shaderObject);
	glLinkProgram(spriteShader._id);

	GLint isLinked = 0;
	glGetProgramiv(spriteShader._id, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		char error[1024];
		glGetProgramInfoLog(spriteShader._id, 1024, nullptr, error);
		printf("[OpenGL shader]: Error with sprite shader linking: %s", error);
	}

	_defaultMaterial.setShaderProgram(spriteShader);

	_triangles = new vec3[_trianglesVertexCapacity];
	_quads = new vec3[_quadsVertexCapacity];
	_spriteTexCoordArray = new vec2[_quadsVertexCapacity];
	
	// Hopefully did the memory right
	size_t vec2Size = sizeof(vec2);
	for (size_t i = 0; i < _quadsVertexCapacity; i += 4)
	{
		memcpy(_spriteTexCoordArray + i, texCoordinatesVec, 4 * vec2Size);
	}

	_trianglesVertexCount = 0;
	_quadsVertexCount = 0;
	glGenVertexArrays(1, &_trianglesGLArray);
	glGenVertexArrays(1, &_quadsGLArray);

	_currentlyActiveTexture = nullptr;
	_currentMaterial = &_defaultMaterial;

	//Here for historic reasons
	LoadMedia();
}

bool firstTime = true;
float totalTime = 0;

void SpriteRenderer::render(float deltaTime) {
	//totalTime += deltaTime * 20.f;
	

	////Draw triangles
	//glBindVertexArray(_trianglesGLArray);
	//glDrawElements(GL_TRIANGLES, _trianglesVertexCount / 3, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	//
	////Draw quads
	//glBindVertexArray(_quadsGLArray);
	//glDrawElements(GL_QUADS, _quadsVertexCount / 4, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);

	//ENGINE.renderScenes();

	
	//SDL_GL_SwapWindow(gameWindow);
}

void SpriteRenderer::addTriangles(vec3 *vertices, int size) {
	_trianglesVertexCount += size;
	if (size % 3 != 0) {
		// Size must be dividible by 3!
		return;
	}
	for (int i = 0; i < size; i++) {
		++_trianglesVertexCount;
		if (_trianglesVertexCount > _trianglesVertexCapacity) {
			_trianglesVertexCapacity += 50000;
			vec3* newTriangles = new vec3[_trianglesVertexCapacity];
			for (int j = 0; j < _trianglesVertexCapacity - 1; j++) {
				newTriangles[j] = _triangles[j];
			}
			delete[] _triangles;
			_triangles = newTriangles;
		}
		_triangles[_trianglesVertexCount] = *vertices;
	}
}

// TODO: implement passing texture coordinate pointers, so that we can use atlases
void SpriteRenderer::addQuads(vec3 *vertices, int size, MTexture* texture, Material* customMaterial) {
	//_quadsVertexCount += size;
	if (size % 4 != 0) {
		// Size must be dividible by 4!
		return;
	}
	if (_currentlyActiveTexture != texture || _currentMaterial != customMaterial)
	{
		if (_currentMaterial)
		{
			//TODO: resolve the conflict between custom materials for quads and for triangles
			//Preferably, remove quads and draw only triangles
			_currentMaterial->useMaterial();
		}
		drawQuads();
		_currentlyActiveTexture = texture;
		_currentMaterial = customMaterial ? customMaterial : &_defaultMaterial;
	}
	for (int i = 0; i < size; i++) {
		++_quadsVertexCount;
		if (_quadsVertexCount > _quadsVertexCapacity) {
			// Revisit this part
			_quadsVertexCapacity += MAX_QUADS_VERTICES;
			vec3* newQuads = static_cast<vec3*>(malloc(_quadsVertexCapacity * sizeof(vec3)));
			for (int j = 0; j < _quadsVertexCapacity - 1; j++) {
				newQuads[j] = _quads[j];
			}
			delete[] _quads;
			_quads = newQuads;
		}
		_quads[_quadsVertexCount - 1] = *(vertices + i);
	}
}

void SpriteRenderer::preRender() {
}

void SpriteRenderer::postRender() {
	_currentMaterial->useMaterial();
	drawQuads();
	_currentlyActiveTexture = nullptr;
	_currentMaterial = &_defaultMaterial;
}

void SpriteRenderer::drawTriangles() {
	glBindVertexArray(_trianglesGLArray);
	glDrawElements(GL_TRIANGLES, _trianglesVertexCount / 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	_trianglesVertexCount = 0;

}

void SpriteRenderer::drawQuads() {
	if (_quadsVertexCount > 0 && _currentlyActiveTexture) {
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, _spriteTexCoordArray);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, _quads);
		glBindTexture(GL_TEXTURE_2D, _currentlyActiveTexture->_glTexture);
		glDrawArrays(GL_QUADS, 0, _quadsVertexCount);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		_quadsVertexCount = 0;
	}
}
