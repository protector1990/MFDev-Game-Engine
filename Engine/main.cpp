#include "Engine.h"
#include <SDL.h>

int main(int argc, char **argv) {
	Engine* m_Engine = &Engine::getInstance();
	m_Engine->run();
	return 0;
}
