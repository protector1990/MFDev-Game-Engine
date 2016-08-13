/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "GameObjectLoaderFactory.h"
#include "SpriteLoader.h"
#include "NavGridLoader.h"

SpriteLoader spriteLoader;
NavGridLoader navGridLoader;

AbstractGameObjectLoader* GameObjectLoaderFactory::getGameObjectLoader(const char *type) const {
	if (!strcmp(type, "Sprite")) {
		return static_cast<AbstractGameObjectLoader*>(&spriteLoader);
	}
	if (!strcmp(type, "NavGrid"))
	{
		return static_cast<AbstractGameObjectLoader*>(&navGridLoader);
	}
	return nullptr;
}
