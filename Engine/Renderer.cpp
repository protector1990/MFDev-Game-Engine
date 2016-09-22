/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

// Breakpoints that should ALWAYS trigger (EVEN IN RELEASE BUILDS) [x86]!
#ifdef _MSC_VER
#include <windows.h>
# define eTB_CriticalBreakPoint() if (IsDebuggerPresent ()) __debugbreak ();
#else
# define eTB_CriticalBreakPoint() asm (" int $3");
#endif


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



const char*
ETB_GL_DEBUG_SOURCE_STR(GLenum source)
{
	static const char* sources[] = {
		"API", "Window System", "Shader Compiler", "Third Party", "Application",
		"Other", "Unknown"
	};

	int str_idx =
		std::min(source - GL_DEBUG_SOURCE_API,
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
		std::min(type - GL_DEBUG_TYPE_ERROR,
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
		std::min(severity - GL_DEBUG_SEVERITY_HIGH,
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
		std::min(severity - GL_DEBUG_SEVERITY_HIGH,
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

SDL_Window *gameWindow = nullptr;
SDL_Surface *screenSurface = nullptr;
SDL_Surface *image = nullptr;
Asset3D* model;
Model3D* meshModel;
SDL_GLContext glContext;

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

void Renderer::init() {
	//renderer initialization
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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
	if (glDebugMessageControlARB != NULL) {
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		glDebugMessageCallbackARB((GLDEBUGPROCARB)ETB_GL_ERROR_CALLBACK, NULL);
	}

	// Sprite shader
	_spriteShaderProgram = new ShaderProgram();

	_spriteShaderProgram->_frag = ASSET_MANAGER->loadAsset<Shader>("/shaders/FSpriteShader.glsl");
	_spriteShaderProgram->_vert = ASSET_MANAGER->loadAsset<Shader>("/shaders/VSpriteShader.glsl");
	_spriteShaderProgram->_id = glCreateProgram();
	glAttachShader(_spriteShaderProgram->_id, _spriteShaderProgram->_vert->_shaderObject);
	glAttachShader(_spriteShaderProgram->_id, _spriteShaderProgram->_frag->_shaderObject);
	glLinkProgram(_spriteShaderProgram->_id);

	GLint isLinked = 0;
	glGetProgramiv(_spriteShaderProgram->_id, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		char error[1024];
		glGetProgramInfoLog(_spriteShaderProgram->_id, 1024, nullptr, error);
		printf("[OpenGL shader]: Error with sprite shader linking: %s", error);
	}
	_triangles = new vec3[_trianglesVertexCapacity];
	_quads = new vec3[_quadsVertexCapacity];
	_spriteTexCoordArray = new vec2[_quadsVertexCapacity];
	
	// Hopefully did the memory right
	size_t vec2Size = sizeof(vec2);
	for (size_t i = 0; i < _quadsVertexCapacity; i += 4)
	{
		memcpy(_spriteTexCoordArray + i, texCoordinatesVec, 4 * vec2Size);
	}

	for (int i = 0; i < 4; ++i)
	{
		printf("%f %f\n", _spriteTexCoordArray[i].x, _spriteTexCoordArray[i].y);
	}

	_trianglesVertexCount = 0;
	_quadsVertexCount = 0;
	glGenVertexArrays(1, &_trianglesGLArray);
	glGenVertexArrays(1, &_quadsGLArray);

	//printf("%p dsdsf\n\n\n", glGetError());

	//SDL_GL_SetSwapInterval(1);
	//screenSurface = SDL_GetWindowSurface(gameWindow);
	//glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glOrtho(-320., 320, -192, 192, -100., 100.);
	//gluLookAt(
	//	5, 5, 5,
	//	0, 0, 0,
	//	-1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//mat4x4 mat = lookAt(vec3(5, 5, 5), vec3(-1, -1, -1), vec3(-1, 1, -1));
	//glLoadMatrixf(&mat[0][0]);
	//glCullFace(GL_BACK);
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glShadeModel(GL_SMOOTH);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glFrontFace(GL_CW);

	LoadMedia();
	//printf("%i %i", x, y);
}

bool firstTime = true;
float totalTime = 0;

void Renderer::render(float deltaTime) {
	//totalTime += deltaTime * 20.f;
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	////Draw triangles
	//glBindVertexArray(_trianglesGLArray);
	//glDrawElements(GL_TRIANGLES, _trianglesVertexCount / 3, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	//
	////Draw quads
	//glBindVertexArray(_quadsGLArray);
	//glDrawElements(GL_QUADS, _quadsVertexCount / 4, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);

	ENGINE.renderScenes();

	drawQuads();

	glFlush();
	SDL_GL_SwapWindow(gameWindow);
}

void Renderer::addTriangles(vec3 *vertices, int size) {
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

// TODO: move this to Renderer class
MTexture* _currentlyActiveTexture;

void Renderer::addQuads(vec3 *vertices, int size, MTexture* texture) {
	//_quadsVertexCount += size;
	if (size % 4 != 0) {
		// Size must be dividible by 4!
		return;
	}
	if (_currentlyActiveTexture != texture)
	{
		drawQuads();
		_currentlyActiveTexture = texture;
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

void Renderer::drawTriangles() {
	glBindVertexArray(_trianglesGLArray);
	glDrawElements(GL_TRIANGLES, _trianglesVertexCount / 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	_trianglesVertexCount = 0;

}

void Renderer::drawQuads() {
	if (_quadsVertexCount > 0 && _currentlyActiveTexture) {
		//glUseProgram(_spriteShaderProgram->_id);

		glEnable(GL_TEXTURE_2D);
		//printf("%p\n\n\n", glGetError());

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//printf("%p\n\n\n", glGetError());

		glTexCoordPointer(2, GL_FLOAT, 0, _spriteTexCoordArray);
		//printf("%p\n\n\n", glGetError());

		glEnableClientState(GL_VERTEX_ARRAY);
		//printf("%p\n\n\n", glGetError());
		
		glVertexPointer(3, GL_FLOAT, 0, _quads);
		//printf("%p\n\n\n", glGetError());
		
		glActiveTexture(GL_TEXTURE0);
		//printf("%p\n\n\n", glGetError());

		glClientActiveTexture(GL_TEXTURE0);
		//printf("%p\n\n\n", glGetError());

		glBindTexture(GL_TEXTURE_2D, _currentlyActiveTexture->_glTexture);
		//printf("%p\n\n\n", glGetError());

		glDrawArrays(GL_QUADS, 0, _quadsVertexCount * sizeof(vec2));
		//vec2* scnd = _spriteTexCoordArray + 1;
		//("%p\n\n\n", glGetError());
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		//glUseProgram(0);
		_quadsVertexCount = 0;
	}
}
