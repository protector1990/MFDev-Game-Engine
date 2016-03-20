//#define GL_GLEXT_PROTOTYPES
#include <GL\glew.h>
//#include <GL\glxew.h>
#include "Engine.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#include "3DAsset.h"
#include <..\include\assimp\scene.h>

using namespace std;

SDL_Window *gameWindow = 0;
SDL_Surface *screenSurface = 0;
SDL_Surface *image = 0;
Asset3D* model;
SDL_GLContext glContext;

#define ASSET_MANAGER Engine::getInstance().getAssetManager()

//helper function
void LoadMedia(){
	image = ASSET_MANAGER->loadAsset<SDL_Surface>("/textures/ZnamSDL.bmp");
	model = ASSET_MANAGER->loadAsset<Asset3D>("/models/house.fbx");
	aiMesh mesh;
	aiVector3D vec;
	GLuint x;
	glGenBuffers(1, &x);
}

void Renderer::init() {
	//renderer initialization
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	gameWindow = SDL_CreateWindow("Game Window", 300, 150, 400, 400, SDL_WINDOW_OPENGL);
	glContext = SDL_GL_CreateContext(gameWindow);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	SDL_GL_SetSwapInterval(1);
	//screenSurface = SDL_GetWindowSurface(gameWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	int x = 0;
	int y = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &x);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &y);
	printf("%i %i", x, y);
}

void Renderer::render(float deltaTime) {
	//SDL_BlitSurface(image, 0, screenSurface, 0);
	glColor3f(.5f, .5f, .5f);
	glBegin(GL_LINES);
	glVertex3f(0.2f, 0.7f, 1.f);
	glVertex3f(0.5f, 0.1f, 1.f);
	glEnd();
	glFlush();
	SDL_GL_SwapWindow(gameWindow);
	//SDL_UpdateWindowSurface(gameWindow);
}