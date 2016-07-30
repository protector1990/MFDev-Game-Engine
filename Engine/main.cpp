/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Engine.h"
#include <SDL.h>

int main(int argc, char **argv) {
	Engine* m_Engine = &Engine::getInstance();
	m_Engine->run(argc, argv);
	return 0;
}
