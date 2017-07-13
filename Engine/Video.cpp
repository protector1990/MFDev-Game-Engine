/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#pragma once

#include "Common.h"
#include "Video.h"
#include <algorithm>
#include "Camera.h"
#include <SDL_system.h>

using namespace std;

// Breakpoints that should ALWAYS trigger (EVEN IN RELEASE BUILDS) [x86]!
#ifdef _MSC_VER
#include <windows.h>
# define eTB_CriticalBreakPoint() if (IsDebuggerPresent ()) __debugbreak ();
#else
# define eTB_CriticalBreakPoint() asm (" int $3");
#endif

const char*
ETB_GL_DEBUG_SOURCE_STR(GLenum source)
{
	static const char* sources[] = {
		"API", "Window System", "Shader Compiler", "Third Party", "Application",
		"Other", "Unknown"
	};

	int str_idx =
		min(source - GL_DEBUG_SOURCE_API,
		sizeof(sources) / sizeof(const char *));

	return sources[str_idx];
}

const char*
ETB_GL_DEBUG_TYPE_STR(GLenum type)
{
	static const char* types[] = {
		"Error", "Deprecated Behavior", "Undefined Behavior", "Portability",
		"Performance", "Other", "Unknown"
	};

	int str_idx =
		min(type - GL_DEBUG_TYPE_ERROR,
		sizeof(types) / sizeof(const char *));

	return types[str_idx];
}

const char*
ETB_GL_DEBUG_SEVERITY_STR(GLenum severity)
{
	static const char* severities[] = {
		"High", "Medium", "Low", "Unknown"
	};

	int str_idx =
		min(severity - GL_DEBUG_SEVERITY_HIGH,
		sizeof(severities) / sizeof(const char *));

	return severities[str_idx];
}

DWORD
ETB_GL_DEBUG_SEVERITY_COLOR(GLenum severity)
{
	static DWORD severities[] = {
		0xff0000ff, // High (Red)
		0xff00ffff, // Med  (Yellow)
		0xff00ff00, // Low  (Green)
		0xffffffff  // ???  (White)
	};

	int col_idx =
		min(severity - GL_DEBUG_SEVERITY_HIGH,
		sizeof(severities) / sizeof(DWORD));

	return severities[col_idx];
}

void
ETB_GL_ERROR_CALLBACK(GLenum        source,
GLenum        type,
GLuint        id,
GLenum        severity,
GLsizei       length,
const GLchar* message,
GLvoid*       userParam)
{
	printf("OpenGL Error:\n");
	printf("=============\n");

	printf(" Object ID: ");
	printf("%d\n", id);

	printf(" Severity:  ");
	printf(
		"%s\n",
		ETB_GL_DEBUG_SEVERITY_STR(severity));

	printf(" Type:      ");
	printf("%s\n", ETB_GL_DEBUG_TYPE_STR(type));

	printf(" Source:    ");
	printf("%s\n", ETB_GL_DEBUG_SOURCE_STR(source));

	printf(" Message:   ");
	printf("%s\n\n", message);

	// Force the console to flush its contents before executing a breakpoint
	//eTB_FlushConsole();

	// Trigger a breakpoint in gDEBugger...
	glFinish();

	// Trigger a breakpoint in traditional debuggers...
	eTB_CriticalBreakPoint();
}


void Video::init() {
	//video initialization
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	_gameWindow = SDL_CreateWindow("Game Window", 300, 150, 640, 384, SDL_WINDOW_OPENGL);
	_glContext = SDL_GL_CreateContext(_gameWindow);


	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %p\n", glewGetErrorString(err));
	}

	// SUPER VERBOSE DEBUGGING!
	if (glDebugMessageControl != NULL) {
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		glDebugMessageCallback((GLDEBUGPROCARB)ETB_GL_ERROR_CALLBACK, NULL);
	}

	glCullFace(GL_BACK);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//glMatrixMode(GL_PROJECTION);
	//glOrtho(-320, 320, -192, 192, -10, 10);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_spriteRenderer->init();
	_renderers.push_back(_spriteRenderer);

	// TODO: make camera system to handle this
	glOrtho(-320., 320, -192, 192, -100., 100.);
}

void Video::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	const std::vector<Scene*>* scenes = ENGINE.getScenes();

	for (Camera* camera : _cameras) {
		if (!camera->getEnabled()) continue;

		camera->preRender();
	
		for (Renderer* renderer : _renderers)
		{
			renderer->preRender();
		}

		for (unsigned int i = 0; i < scenes->size(); i++) {
			if ((*scenes)[i]->getActive()) {
				(*scenes)[i]->render();
			}
		}

		for (Renderer* renderer : _renderers)
		{
			renderer->postRender();
		}

		camera->postRender();
	}

	glFlush();
	SDL_GL_SwapWindow(_gameWindow);
}

void Video::addCamera(Camera* camera) {
	_cameras.push_front(camera);
	_cameras.sort([](const Camera* cam1, const Camera *cam2) -> bool {
		return cam1->getRenderingOrder() < cam2->getRenderingOrder();
	});
}

void Video::removeCamera(Camera* camera) {
	_cameras.remove(camera);
}

glm::ivec2 Video::getWindowSize(){
	glm::ivec2 ret;
	SDL_GetWindowSize(_gameWindow, &ret.x, &ret.y);
	return ret;
}

Video::Video(): _glContext(nullptr) {
}
