/** Copyright 2016 MarFil Studios **/

#ifndef _GameObjectLoaderFactory_H
#define _GameObjectLoaderFactory_H

//Game Object loaders dependency injection

#include "AbstractGameObjectLoader.h"

class GameObjectLoaderFactory {
	AbstractGameObjectLoader* getGameObjectLoader(const char *type);
};
#endif