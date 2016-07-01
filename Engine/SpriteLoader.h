/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _SPRITE_LOADER_H
#define _SPRITE_LOADER_H

#include "AbstractGameObjectLoader.h"

class SpriteLoader : public AbstractGameObjectLoader {
public:
	GameObject* load(rapidxml::xml_node<char>* configuration) override;
};
#endif