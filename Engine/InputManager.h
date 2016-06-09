/**  Copyright 2016 MarFil studios  **/
#include "Common.h"
#include <set>

class InputManager {
public:
	bool queryKeyDown(char key);
	void keyPressed(char key);
	void keyReleased(char key);
protected:
	//TODO: Use some kind of map
	std::set<char> _keysDown;

};