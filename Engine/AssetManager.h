/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#ifndef _ASSET_MANAGER_H
#define _ASSET_MANAGER_H

#include <SDL_platform.h>
#include <SDL.h>
#include "Lua.h"
#include "3DAsset.h"
#include "Model3D.h"
#include "Scene.h"
#include "TextAsset.h"
#include "Shader.h"
#include "Texture.h"

//TODO: In current implementation, Asset Manager and Asset Loader funcionalities are inside the same class.
// Set those tho apart.
class AssetManager {
public:
	AssetManager(){}
	virtual ~AssetManager(){}
	
	virtual void init() = 0;
	virtual void deInit() = 0;

	template <typename T>
	T* loadAsset(const char *path);
public:
	// Maybe drop the templates, because there must be an additional helper function call and
	// junk code added
	template <> int* loadAsset<int>(const char *path) { return loadInt(path); };
	template <> SDL_Surface* loadAsset<SDL_Surface>(const char *path){return loadSDL_Surface(path);}
	template <> Asset3D* loadAsset<Asset3D>(const char *path){ return loadAsset3D(path); }
	template <> Model3D* loadAsset<Model3D>(const char *path){ return loadModel3D(path); }
	template <> Scene* loadAsset<Scene>(const char *path){ return loadScene(path); }
	template <> TextAsset* loadAsset<TextAsset>(const char *path){ return loadTextAsset(path); }
	// Scripts
	template <> Script* loadAsset<Script>(const char *path){ return loadLuaScript(path); }
	template <> ScriptClass* loadAsset<ScriptClass>(const char *path){ return loadScriptClass(path); }
	// Shaders
	template <> Shader* loadAsset<Shader>(const char *path) { return loadShader(path); }
	template<> MTexture* loadAsset<MTexture>(const char *path) { return loadSpriteTexture(path); }
protected:
	virtual int* loadInt(const char *path) = 0;
	virtual SDL_Surface* loadSDL_Surface(const char *path) = 0;
	virtual Asset3D* loadAsset3D(const char *path) = 0;
	virtual Model3D* loadModel3D(const char *path) = 0;
	virtual Scene* loadScene(const char *path) = 0;
	virtual TextAsset* loadTextAsset(const char *path);

	virtual Script* loadLuaScript(const char *path) = 0;
	virtual ScriptClass* loadScriptClass(const char *path) = 0;
	virtual Shader* loadShader(const char* path) = 0;
	virtual MTexture* loadSpriteTexture(const char *path) = 0;

	/*TODO: this is now a bit irrelevant, because Script no longer stores reference to a Lua class. So, now if we 
	  want to load an already loaded class, load script will return loaded script and class parser would still try to parse it
	  Also, implement as a hash map.
	*/
	std::vector<Script*> _loadedScripts;
	std::vector<Shader*> _loadedShaders;
	std::vector<MTexture*> _loadedTextures;

};

#ifdef __WINDOWS__
class WinAssetManager : public AssetManager {
public:
	void init() override;
	void deInit() override;
protected:
	int* loadInt(const char *path) override;
	SDL_Surface* loadSDL_Surface(const char *path) override;
	Asset3D* loadAsset3D(const char *path) override;
	Model3D* loadModel3D(const char *path) override;
	Scene* loadScene(const char *path) override;

	Script* loadLuaScript(const char *path) override;
	ScriptClass* loadScriptClass(const char *path) override;

	Shader* loadShader(const char *path) override;
	MTexture* loadSpriteTexture(const char *path) override;
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