/** Copyright 2016 MarFil Studios. All rights reserved. **/

#pragma once

#include <vector>
#include "SpriteRenderer.h"
#include "Lua.h"
#include "ScriptableInterface.h"
#include <typeinfo>
#include "Transform.h"
#include "Material.h"
//#include "Component.h"
//typedef SpriteRenderer SpriteRenderer;

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
	//TODO: integrate transform and Game Object properly
	friend class Transform;
	friend class AbstractGameObjectLoader;
public:
	GameObject()
		: _tag(0), _transform(this) {
	}

	virtual ~GameObject();

	//GameObject();
	unsigned int getTag() const;
	void setTag(unsigned int tag);
	bool compareTagAny(unsigned int tag) const;
	bool compareTagAll(unsigned tag) const;
	void appendTag(unsigned int tag);
	//virtual void load(const char* cfgPath) = 0;
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void updateComponents(float deltaTime);
	virtual void render() = 0;
	virtual void renderDebugComponents(float deltaTime);

	virtual GameObject *getRoot();
	virtual GameObject *getParent();
	void setParent(GameObject* newParent);
	virtual GameObject *getChildren(int &count);

	template<class T> T* getChildPolymorphic() const;
	template<class T> T* getChild() const;

	//TODO: Implement that transform system already!
	glm::vec3 _Position;

	Transform _transform;

	ScriptComponent* getLuaComponent(const char* scriptName);
	std::vector<ScriptComponent*> _scriptComponents;
	//Why again we have this function?
	virtual void keyPressed(int keyPressed);

	const ScriptableInterface* getScriptableIterface() const;
	const std::vector<Component*>* getComponentsConst() const;

	const char* getName() const;
	inline bool GameObject::getEnabled() const {return enabled; }

	inline void GameObject::setEnabled(bool value) { enabled = value; }

	void addMaterial(Material* material);
	Material* getMaterial(size_t index);

protected:
	char* _name;
	unsigned int _tag;
	GameObject *_parent = nullptr;
	std::vector<GameObject*> _children;
	std::vector<Component*> _components;
	// This might benefit from having setter and not being const, because we shall yet to see to the problem of loading and unloading scenes
	// and object migration between them.
	Scene *_scene;
	// Ovo ne mo�e ovako. Scriptable interface ne treba svaka instanca da ima svoje funkcije, nego treba da budu po klasi game objekta.
	//Videti kako ovo mo�e da se re�i
	ScriptableInterface _scriptableInterface;
	// TODO: find the most appropriate place for this
	std::vector<Material*> _materials;
	bool enabled = true;
};

template <class T>
T* GameObject::getChildPolymorphic() const
{
	size_t size = _children.size();
	for (size_t i = 0; i < size; ++i)
	{
		T* cast = dynamic_cast<T*>(_children[i]);
		if (cast) return cast;
	}
	return nullptr;
}

template <class T>
T* GameObject::getChild() const
{
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
