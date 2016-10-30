/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

//replace guards in other files with #pragma once
#pragma once

#include "Renderer.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include <vector>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Engine;

#pragma region

inline void utilLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
	//glPushMatrix();
	glm::mat4 lookat = glm::lookAt(eye, center, up);
	glLoadMatrixf(&lookat[0][0]);
}

#pragma endregion

class Video {
public:
	void init();
	MeshRenderer* getMeshRenderer();
	SpriteRenderer* getSpriteRenderer();
	void render();
private:
	MeshRenderer* _meshRenderer = 0;
	SpriteRenderer* _spriteRenderer = 0;
	Video() {}
	std::vector<Renderer*> _renderers;
	SDL_Window *gameWindow = nullptr;
	SDL_GLContext glContext;

	friend class Engine;
public:
	//SDL_videoInfo* sdlInfo;
	Video(Video const&) = delete;
	void operator=(Video const&) = delete;
};