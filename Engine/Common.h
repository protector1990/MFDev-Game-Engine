/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _COMMON_H
#define _COMMON_H

#include "Engine.h"
#include <hash_map>

// Common shortcuts
#define ENGINE Engine::getInstance()
#define ASSET_MANAGER ENGINE.getAssetManager()
#define SCRIPT_MANAGER ENGINE.getLuaManager()

//Predefined tags
#define TAG_NAV_OBSTACLE 1u
//#define TAG_NEXT 2u
//#define TAG_NEXT 4u
//#define TAG_NEXT 8u
//#define TAG_NEXT 16u
//#define TAG_NEXT 32u
//#define TAG_NEXT 64u
//#define TAG_NEXT 128u
//#define TAG_NEXT 256u

#endif