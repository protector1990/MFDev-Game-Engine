/**  Copyright 2016 MarFil Studios  **/

#ifndef _ASSET_MANAGER_H
#define _ASSET_MANAGER_H

#include <SDL_platform.h>
#include <SDL.h>
#include "Lua.h"
#include "3DAsset.h"
#include "Model3D.h"
#include "Scene.h"

class AssetManager {
public:
	AssetManager(){}
	~AssetManager(){}
	
	virtual void init() = 0;
	virtual void deInit() = 0;

	template <typename T>
	T* loadAsset(const char *path);
public:
	// Maybe forget the template, since because it cannot be overriden, there must be additional helper function call and
	// junk code added
	template <> int* loadAsset<int>(const char *path) { return loadInt(path); };
	template <> SDL_Surface* loadAsset<SDL_Surface>(const char *path){return loadSDL_Surface(path);};
	template <> Script* loadAsset<Script>(const char *path){ return loadLuaScript(path); };
	template <> Asset3D* loadAsset<Asset3D>(const char *path){ return loadAsset3D(path); };
	template <> Model3D* loadAsset<Model3D>(const char *path){ return loadModel3D(path); };
	template <> Scene* loadAsset<Scene>(const char *path){ return loadScene(path); };
protected:
	virtual int* loadInt(const char *path) = 0;
	virtual SDL_Surface* loadSDL_Surface(const char *path) = 0;
	virtual Script* loadLuaScript(const char *path) = 0;
	virtual Asset3D* loadAsset3D(const char *path) = 0;
	virtual Model3D* loadModel3D(const char *path) = 0;
	virtual Scene* loadScene(const char *path) = 0;

};

#ifdef __WINDOWS__
class WinAssetManager : public AssetManager {
public:
	void init() override;
	void deInit() override;
protected:
	int* loadInt(const char *path) override;
	SDL_Surface* loadSDL_Surface(const char *path) override;
	Script* loadLuaScript(const char *path) override;
	Asset3D* loadAsset3D(const char *path) override;
	Model3D* loadModel3D(const char *path) override;
	Scene* loadScene(const char *path) override;

};
#endif

#ifdef __IOS__
class IOSAssetManager : public AssetManager {
public:
	void init() override;
	void deInit() override;
protected:
	int* loadInt(const char *path) override;
	SDL_Surface* loadSDL_Surface(const char *path) override;
	Script* loadLuaScript(const char *path) override;
	Asset3D* loadAsset3D(const char *path) override;
	Model3D* loadModel3D(const char *path) override;
	Scene* loadScene(const char *path) override;
};
#endif

#endif