/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _GAME_OBJECT_LOADER_FACTORY_H
#define _GAME_OBJECT_LOADER_FACTORY_H

//Game Object loaders dependency injection

#include "AbstractGameObjectLoader.h"

class GameObjectLoaderFactory {
public:
	AbstractGameObjectLoader* getGameObjectLoader(const char *type) const;
};
#endif