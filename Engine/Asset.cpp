/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#include "Asset.h"

void Asset::grab() {
	if (!_lock)
	{
		load();
	}
	++_lock;
}

void Asset::release() {
	if (!--_lock)
	{
		unload();
	}
}

void Asset::load() {
}

void Asset::unload() {
}
