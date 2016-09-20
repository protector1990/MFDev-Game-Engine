/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#ifndef _SHADER_H
#define _SHADER_H

#include <SDL_opengl.h>

enum ShaderType {
	VERTSHADER,
	FRAGSHADER,
	GEOMSHADER
};

class Shader {
public:
	char* _name;
	ShaderType _type;
	GLuint _location;
	GLuint _shaderObject;
	// see if we need a bool indicating if it is compiled or not
};

#endif // #ifndef _SHADER_H