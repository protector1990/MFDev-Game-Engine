#include "GameObject.h"

class Sprite : public GameObject {
public:
	void load(const char* cfgPath) override;
	void init() override;
	void update(float deltaTime) override;
	void render() override;
};