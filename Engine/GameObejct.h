/** Copyright 2016 MarFil Studios **/

extern class Scene;

// Abstract game object class. Should be implemented as needed for specific game 
// requirements. You can go as far as programming every single game object
// as separate child class, or have a more generic game object implementation
class GameObject {
	friend class Scene;
public:
	GameObject(Scene* parentScene);
	unsigned int getTag();
	void setTag(unsigned int tag);
	Scene* getParentScene();
	virtual void load(const char* cfgPath) = 0;
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
protected:
	unsigned int tag;
};