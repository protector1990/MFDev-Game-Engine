/** Copyright 2016 MarFil Studios **/

#include <vector>

// Abstract game object class. Should be implemented as needed for specific game 
// requirements. You can go as far as programming every single game object
// as separate child class, or have a more generic game object implementation
// For now Godot-style single component objects are preferred
class GameObject {
public:
	GameObject();
	unsigned int getTag();
	void setTag(unsigned int tag);
	virtual void load(const char* cfgPath) = 0;
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;

	virtual GameObject *getRoot();
	virtual GameObject *getParent();
	virtual GameObject *getChildren(int &count);

	template<class T> T* getChild();

protected:
	unsigned int _tag;
	GameObject *_parent;
	std::vector<GameObject*> _children;
};