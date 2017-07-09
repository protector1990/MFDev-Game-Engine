/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#pragma once

#include "Common.h"
#include "Video.h"
#include <algorithm>

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

	gameWindow = SDL_CreateWindow("Game Window", 300, 150, 640, 384, SDL_WINDOW_OPENGL);
	glContext = SDL_GL_CreateContext(gameWindow);


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

	//SpriteRenderer* spriteRenderer = new SpriteRenderer();
	//spriteRenderer->init();
	//
	//_renderers.push_back(spriteRenderer);

	_spriteRenderer = new SpriteRenderer();
	_spriteRenderer->init();
	_renderers.push_back(_spriteRenderer);
}

MeshRenderer* Video::getMeshRenderer() {
	if (!_meshRenderer)
	{
		_meshRenderer = new MeshRenderer();
		_meshRenderer->init();
		_renderers.push_back(_meshRenderer);
	}
	return _meshRenderer;
}

SpriteRenderer* Video::getSpriteRenderer() {
	if (!_spriteRenderer)
	{
		_spriteRenderer = new SpriteRenderer();
		_spriteRenderer->init();
		_renderers.push_back(_spriteRenderer);
	}
	return _spriteRenderer;
}

void Video::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	for (Renderer* renderer : _renderers)
	{
		renderer->preRender();
	}

	ENGINE.renderScenes();

	for (Renderer* renderer : _renderers)
	{
		renderer->postRender();
	}

	glFlush();
	SDL_GL_SwapWindow(gameWindow);
}
