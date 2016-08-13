/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _NAV_GRID_LOADER_H
#define _NAV_GRID_LOADER_H

#include "AbstractGameObjectLoader.h"

class NavGridLoader : public AbstractGameObjectLoader {

public:
	GameObject* load(rapidxml::xml_node<char>* configuration) override;
};

#endif // #ifndef _NAV_GRID_LOADER_H