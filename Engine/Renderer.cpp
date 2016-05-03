
#include "Engine.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "3DAsset.h"
#include <..\include\assimp\scene.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Sprite.h"

using namespace std;
using namespace glm;

SDL_Window *gameWindow = 0;
SDL_Surface *screenSurface = 0;
SDL_Surface *image = 0;
Asset3D* model;
Model3D* meshModel;
SDL_GLContext glContext;

#define ASSET_MANAGER Engine::getInstance().getAssetManager()
#define ENGINE Engine::getInstance()

//helper function
void LoadMedia() {
	image = ASSET_MANAGER->loadAsset<SDL_Surface>("/textures/ZnamSDL.bmp");
	//model = ASSET_MANAGER->loadAsset<Asset3D>("/models/house.fbx");
	meshModel = ASSET_MANAGER->loadAsset<Model3D>("/models/path.fbx");
	meshModel->setUp();	
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
	gameWindow = SDL_CreateWindow("Game Window", 300, 150, 1280, 768, SDL_WINDOW_OPENGL);
	glContext = SDL_GL_CreateContext(gameWindow);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	_triangles = new vec3[_trianglesVertexCapacity];
	_quads = new vec3[_quadsVertexCapacity];
	_trianglesVertexCount = 0;
	_quadsVertexCount = 0;

	glGenVertexArrays(sizeof(vec3), &_trianglesGLArray);
	glGenVertexArrays(sizeof(vec3), &_quadsGLArray);

	//SDL_GL_SetSwapInterval(1);
	//screenSurface = SDL_GetWindowSurface(gameWindow);
	//glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glOrtho(-8.3, 8.3, -5, 5, -10, 10);
	//gluLookAt(
	//	5, 5, 5,
	//	0, 0, 0,
	//	-1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//mat4x4 mat = lookAt(vec3(5, 5, 5), vec3(-1, -1, -1), vec3(-1, 1, -1));
	//glLoadMatrixf(&mat[0][0]);
	glCullFace(GL_BACK);
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_COLOR_MATERIAL);
	LoadMedia();
	//printf("%i %i", x, y);
}

bool firstTime = true;
float totalTime = 0;

void Renderer::render(float deltaTime) {
	totalTime += deltaTime * 20.f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Draw triangles
	glBindVertexArray(_trianglesGLArray);
	glDrawElements(GL_TRIANGLES, _trianglesVertexCount / 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Draw quads
	glBindVertexArray(_quadsGLArray);
	glDrawElements(GL_QUADS, _quadsVertexCount / 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glFlush();
	SDL_UpdateWindowSurface(gameWindow);
}

//Will be mostly used for sprites
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
			delete[_trianglesVertexCount - 1] _triangles;
			_triangles = newTriangles;
		}
		_triangles[_trianglesVertexCount] = *vertices;
	}
}

void Renderer::addQuads(vec3 *vertices, int size) {
	_quadsVertexCount += size;
	if (size % 4 != 0) {
		// Size must be dividible by 4!
		return;
	}
	for (int i = 0; i < size; i++) {
		++_quadsVertexCount;
		if (_quadsVertexCount > _quadsVertexCapacity) {
			_quadsVertexCapacity += 50000;
			vec3* newQuads = new vec3[_quadsVertexCapacity];
			for (int j = 0; j < _quadsVertexCapacity - 1; j++) {
				newQuads[j] = _quads[j];
			}
			delete[_quadsVertexCount - 1] _quads;
			_quads = newQuads;
		}
		_quads[_quadsVertexCount] = *vertices;
	}
}

