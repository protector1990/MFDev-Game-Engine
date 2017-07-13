/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "LoaderFactories.h"
#include "SpriteLoader.h"
#include "NavGridLoader.h"
#include "AreaLoader.h"
#include "CameraLoader.h"

//TODO: Refactor these so they start using osme sort of a hash map

SpriteLoader spriteLoader;
NavGridLoader navGridLoader;
CameraLoader cameraLoader;

AreaLoader areaLoader;

AbstractGameObjectLoader* GameObjectLoaderFactory::getGameObjectLoader(const char *type) const {
	if (!strcmp(type, "Sprite")) {
		return static_cast<AbstractGameObjectLoader*>(&spriteLoader);
	}
	if (!strcmp(type, "NavGrid"))
	{
		return static_cast<AbstractGameObjectLoader*>(&navGridLoader);
	}
	if (!strcmp(type, "Camera"))
	{
		return static_cast<AbstractGameObjectLoader*>(&cameraLoader);
	}
	return nullptr;
}

AbstractComponentLoader* ComponentLoaderFactory::getComponentLoader(const char* type) const {
	if (!strcmp(type, "Area")) {
		return static_cast<AbstractComponentLoader*>(&areaLoader);
	}
	return nullptr;
}
