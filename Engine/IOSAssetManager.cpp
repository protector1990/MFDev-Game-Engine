#include "AssetManager.h"

#ifdef __IOS__
template <int> int* AssetManager::loadAsset(const char *path) {
	//implement this
	int i = 0;
	return &i;
}

template <SDL_Surface> SDL_Surface* AssetManager::loadAsset(const char *path) {

}
#endif