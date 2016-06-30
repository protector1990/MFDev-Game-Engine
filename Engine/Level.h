/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _LEVEL_H
#define _LEVEL_H

#include "Scene.h"

class Level : public Scene {
public:
	Scene* load(const char *cfgPath, bool parallel) override;
	bool unload(Scene *scene) override;
	void update(float deltaTime) override;
};

#endif