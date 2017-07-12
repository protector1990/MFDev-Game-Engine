/** Copyright 2017 MarFil studios. All rights reserved. **/
#pragma once

#include "AbstractGameObjectLoader.h"

class CameraLoader : public AbstractGameObjectLoader {
public:
	virtual GameObject* load(rapidxml::xml_node<char>* configuration) final;
};