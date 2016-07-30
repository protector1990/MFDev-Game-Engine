/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "GameObjectLoaderFactory.h"
#include "SpriteLoader.h"

SpriteLoader spriteLoader;

AbstractGameObjectLoader* GameObjectLoaderFactory::getGameObjectLoader(const char *type) const {
	if (strcmp(type, "sprite")) {
		return static_cast<AbstractGameObjectLoader*>(&spriteLoader);
	}
	return nullptr;
}
