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
#include <forward_list>

class Engine;
class Camera;

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
	void render();
	MeshRenderer* const _meshRenderer = new MeshRenderer();
	SpriteRenderer* const _spriteRenderer = new SpriteRenderer();
	void addCamera(Camera* camera);
	void removeCamera(Camera* camera);
	glm::ivec2 getWindowSize();
private:
	Video();
	std::vector<Renderer*> _renderers;
	SDL_Window *_gameWindow = nullptr;
	SDL_GLContext _glContext;
	std::forward_list<Camera*> _cameras;
	friend class Engine;
public:
	//SDL_videoInfo* sdlInfo;
	Video(Video const&) = delete;
	void operator=(Video const&) = delete;
};