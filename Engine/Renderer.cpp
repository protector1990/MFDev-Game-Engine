
#include <GL\glew.h>
#include "Engine.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "3DAsset.h"
#include <..\include\assimp\scene.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\matrix_transform.hpp>

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
	glRotatef(totalTime, 0.f, totalTime, 1.f);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	meshModel->draw();
	glFlush();
	SDL_GL_SwapWindow(gameWindow);
	firstTime = false;
	SDL_UpdateWindowSurface(gameWindow);
}