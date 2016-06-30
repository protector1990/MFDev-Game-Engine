/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "GameObjectLoaderFactory.h"
#include "SpriteLoader.h"

SpriteLoader spriteLoader;

AbstractGameObjectLoader* GameObjectLoaderFactory::getGameObjectLoader(const char *type) {
	if (strcmp(type, "sprite")) {
		return (AbstractGameObjectLoader*)&spriteLoader;
	}
}