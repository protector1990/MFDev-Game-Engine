/** Copyright 2016 MarFil Studios. All rights reserved. **/

#pragma once

#include "AbstractComponentLoader.h"

class AreaLoader : public AbstractComponentLoader {
public:
	Component* loadComponent(rapidxml::xml_node<char>* configuration, GameObject* parentObject) override;
};
