/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <vector>
#include "Renderer.h"
#include "Lua.h"
#include "ScriptableInterface.h"
#include <typeinfo>
//#include "Component.h"
//typedef Renderer Renderer;

class Scene;
class Component;

// Abstract game object class. Should be implemented as needed for specific game 
// requirements. You can go as far as programming every single game object
// as separate child class, or have a more generic game object implementation
// For now Unity componented style objects are performed, although every new object type must be
// defined as a c++ class. That enables game objects to have some predefined behaviours (differs from
// Unity, where complete GameObject behaviour is defined through its components
class GameObject {
	//TODO: refactor this so that there is a loader class that can be friend
	// and Win/iOS asset managers should only concern input methods
	friend class WinAssetManager;
public:
	GameObject()
		: _tag(0) {
	}

	virtual ~GameObject();

	//GameObject();
	unsigned int getTag() const;
	void setTag(unsigned int tag);
	bool compareTag(unsigned int tag) const;
	void appendTag(unsigned int tag);
	//virtual void load(const char* cfgPath) = 0;
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void updateComponents(float deltaTime);
	virtual void render(Renderer *renderer) = 0;
	virtual void renderDebugComponents(float deltaTime);

	virtual GameObject *getRoot();
	virtual GameObject *getParent();
	virtual GameObject *getChildren(int &count);

	template<class T> T* getChildPolymorphic() const;
	template<class T> T* getChild() const;

	//TODO: Implement that transform system already!
	glm::vec3 _Position;

	ScriptComponent* getLuaComponent(const char* scriptName);
	std::vector<ScriptComponent*> _scriptComponents;
	//Why again we have this function?
	virtual void keyPressed(int keyPressed);

	const ScriptableInterface* getScriptableIterface() const;
	const std::vector<Component*>* getComponentsConst() const;

protected:
	unsigned int _tag;
	GameObject *_parent;
	std::vector<GameObject*> _children;
	std::vector<Component*> _components;
	// This might benefit from having setter and not being const, because we shall yet to see to the problem of loading and unloading scenes
	// and object migration between them.
	Scene *_scene;
	// Ovo ne može ovako. Scriptable interface ne treba svaka instanca da ima svoje funkcije, nego treba da budu po klasi game objekta.
	//Videti kako ovo može da se reši
	ScriptableInterface _scriptableInterface;
};

template <class T>
T* GameObject::getChildPolymorphic() const {
	size_t size = _children.size();
	for (size_t i = 0; i < size; ++i)
	{
		T* cast = dynamic_cast<T*>(_children[i]);
		if (cast) return cast;
	}
	return nullptr;
}

template <class T>
T* GameObject::getChild() const {
	size_t size = _children.size();
	const type_info &inf = typeid(T);
	for (size_t i = 0; i < size; ++i)
	{
		if (typeid(_children[i]) == inf)
		{
			return _children[i];
		}
	}
	return nullptr;
}
#endif
