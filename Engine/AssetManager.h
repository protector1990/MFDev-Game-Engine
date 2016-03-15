#include <SDL_platform.h>
#include <SDL.h>

class AssetManager {
public:
	AssetManager(){}
	~AssetManager(){}
	
	virtual void init() = 0;

	template <typename T>
	T* loadAsset(const char *path);
public:
	// Maybe forget the template, since because it cannot be overriden, there must be additional helper function call and
	// junk code added
	template <> int* loadAsset<int>(const char *path) { return loadInt(path); };
	template <> SDL_Surface* loadAsset<SDL_Surface>(const char *path){return loadSDL_Surface(path);};
protected:
	virtual int* loadInt(const char *path) = 0;
	virtual SDL_Surface* loadSDL_Surface(const char *path) = 0;
};

#ifdef __WINDOWS__
class WinAssetManager : public AssetManager {
public:
	void init() override;
protected:
	int* loadInt(const char *path) override;
	SDL_Surface* loadSDL_Surface(const char *path) override;
};
#endif

#ifdef __IOS__
class IOSAssetManager : public AssetManager {
public:
	void init() override;
protected:
	int* loadInt(const char *path) override;
	SDL_Surface* loadSDL_Surface(const char *path) override;
};
#endif