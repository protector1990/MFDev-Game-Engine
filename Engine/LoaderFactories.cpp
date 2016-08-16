/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "LoaderFactories.h"
#include "SpriteLoader.h"
#include "NavGridLoader.h"
#include "AreaLoader.h"

//TODO: Refactor these so they start using osme sort of a hash map

SpriteLoader spriteLoader;
NavGridLoader navGridLoader;

AreaLoader areaLoader;

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

AbstractComponentLoader* ComponentLoaderFactory::getComponentLoader(const char* type) const {
	if (!strcmp(type, "Area")) {
		return static_cast<AbstractComponentLoader*>(&areaLoader);
	}
	return nullptr;
}
