/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _LOADER_FACTORIES_H
#define _LOADER_FACTORIES_H

//Game Object loaders dependency injection

#include "AbstractGameObjectLoader.h"
#include "AbstractComponentLoader.h"

class GameObjectLoaderFactory {
public:
	AbstractGameObjectLoader* getGameObjectLoader(const char *type) const;
};

class ComponentLoaderFactory {
public:
	AbstractComponentLoader* getComponentLoader(const char *type) const;
};
#endif