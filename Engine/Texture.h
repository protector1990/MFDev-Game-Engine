/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <SDL.h>

//TODO: This needs expanding to include all scenarios, like sprites, normal maps, cubemaps, skins etc...
struct MTexture {
	MTexture(SDL_Surface* nativeTexture, const char* name, GLuint glTexture = 0) :
		_nativeTexture(nativeTexture),
		_glTexture(glTexture),
		_name (name)
	{}
	MTexture(const char* name) :
		_name(name)
	{}
	SDL_Surface* _nativeTexture;
	// TODO: make consistent gl pointer naming scheme
	GLuint _glTexture;
	const char* _name;
};

#endif // #ifndef _TEXTURE_H