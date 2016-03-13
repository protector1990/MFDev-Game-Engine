#include "Engine.h"

int Engine::run() {
	_luaInterpreter = luaL_newstate();
	luaBind(_luaInterpreter);
	_renderer = new Renderer();
	_renderer->init();
	//this should be called from a loop when we make one
	_renderer->render();
	return 0;
}