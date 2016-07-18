/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include "Common.h"
#include <set>

class InputManager {
public:
	bool queryKeyDown(char key);
	void keyPressed(char key);
	void keyReleased(char key);
protected:
	//TODO: Use some kind of map
	//We would benefit from having a copy of this in a LUA table somewhere, or lua callbacks \
	  current system for querying for every key in every frame is inefficient.
	std::set<char> _keysDown;

};

#endif