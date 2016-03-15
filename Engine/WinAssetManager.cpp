#include "AssetManager.h"
#include <physfs.h>
#include <string>
#include <stdio.h>

#ifdef __WINDOWS__
	
void WinAssetManager::init() {
	PHYSFS_init(0);
	const char *baseDir = PHYSFS_getBaseDir();
	char *dataPath = new char;// = new char[strlen(baseDir) + 7];
	strcpy(dataPath, baseDir);
	strcat(dataPath, "data.zip");
	size_t index = 0;
	printf("%i", strlen(dataPath));
	printf("%i", PHYSFS_mount(dataPath, "/", 1));
}

int* WinAssetManager::loadInt(const char *path) {
	//implement this
	int i = 0;
	return &i;
}

SDL_Surface* WinAssetManager::loadSDL_Surface(const char *path) {
	PHYSFS_file* file = PHYSFS_openRead(path);
	PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
	char* inBuff = new char[fileSize];
	int length = PHYSFS_read(file, inBuff, PHYSFS_fileLength(file), 1);
	SDL_RWops *rw = SDL_RWFromMem(inBuff, length);
	SDL_Surface *ret = SDL_LoadBMP_RW(rw, 1);
	return ret;
}


#endif