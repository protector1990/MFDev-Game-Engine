/**  Copyright 2016 MarFil Studios  **/

#ifndef ASSET3D
#define ASSET3D
//#include "Mesh.h"
#include <GL\glew.h>
#include <..\include\assimp\scene.h>
#include <SDL_opengl.h>

//Container for aiScene or any other future formats
class Asset3D {
public:
	aiScene* scene;
	void convertMesh(int meshNo);
	void drawMesh(int meshNo);
private:
	//mMesh* mesh;
};

#endif