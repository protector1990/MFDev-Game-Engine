#ifndef _Sprite_H
#define _Sprite_H

#include "GameObject.h"
#include <SDL.h>

class Sprite : public GameObject {
	friend class SpriteLoader;
public:
	void load(const char* cfgPath) override;
	void init() override;
	void update(float deltaTime) override;
	void render() override;
	const SDL_Surface* geTexture();
protected:
	SDL_Surface *_texture;
	int sheetWidth = 1;
	int sheetHeight = 1;
	float animSpeed = 1.f;
};
#endif