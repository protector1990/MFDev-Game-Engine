#define RAPIDXML_NO_EXCEPTIONS
#include "AssetManager.h"
#include <physfs.h>
#include <string>
#include <stdio.h>
#include <..\include\assimp\Importer.hpp>
#include <..\include\assimp\scene.h>
#include <..\include\assimp\postprocess.h>
#include "Physfs2AssimpInterface.h"
#include <..\rapidxml-1.13\rapidxml.hpp>
#include "Sprite.h"
#include "SpriteLoader.h"
#include "Level.h"
#include "GameObjectLoaderFactory.h"

#ifdef __WINDOWS__

using namespace rapidxml;

Assimp::Importer asset3DImporter;
GameObjectLoaderFactory loaderFactory;

namespace rapidxml
{
	void parse_error_handler(const char *what, void *where) {
		printf("%s", what);
	}
}

void WinAssetManager::init() {
	PHYSFS_init(0);
	const char *baseDir = PHYSFS_getBaseDir();
	char *dataPath = new char;// = new char[strlen(baseDir) + 7];
	strcpy(dataPath, baseDir);
	strcat(dataPath, "data.zip");
	PHYSFS_mount(dataPath, "/", 1);
	char *uncompressedDataPath = new char;
	strcpy(uncompressedDataPath, baseDir);
	strcat(uncompressedDataPath, "data");
	PHYSFS_mount(uncompressedDataPath, "/", 1);
	asset3DImporter.SetIOHandler(new P2AIOSystem());
}

void WinAssetManager::deInit() {
	PHYSFS_deinit();
	asset3DImporter.FreeScene();
}

int* WinAssetManager::loadInt(const char *path) {
	//implement this
	int i = 0;
	return &i;
}

SDL_Surface* WinAssetManager::loadSDL_Surface(const char *path) {
	PHYSFS_file* file = PHYSFS_openRead(path);
	if (file) {
		PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
		char* inBuff = new char[fileSize];
		PHYSFS_read(file, inBuff, fileSize, 1);
		SDL_RWops *rw = SDL_RWFromMem(inBuff, fileSize);
		return SDL_LoadBMP_RW(rw, 1);
	}
	else {
		return NULL;
	}
}

LuaScript* WinAssetManager::loadLuaScript(const char *path) {
	PHYSFS_file* file = PHYSFS_openRead(path);
	if (file) {
		PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
		char* inBuff = new char[fileSize];
		PHYSFS_read(file, inBuff, fileSize, 1);
		return new LuaScript(path, inBuff, fileSize);
	}
	else {
		return NULL;
	}
}

// A more generic way of loading 3D assets. Will load whole scene
Asset3D* WinAssetManager::loadAsset3D(const char *path) {
	const aiScene* temp = asset3DImporter.ReadFile(path, 0);
	Asset3D* ret = new Asset3D();
	//We do not want const aiScene
	ret->scene = new aiScene(*temp);
	return ret;
}

// Will only load a single mesh. Intended for simple objects, not for complex scenes
// Finish this to be able to load vertex colors, skeletal animations and so on
Model3D* WinAssetManager::loadModel3D(const char *path) {
	const aiScene* temp = asset3DImporter.ReadFile(path, 0);
	if (!(temp->HasMeshes())) {
		return NULL;
	}
	Model3D* ret = new Model3D();
	aiMesh* currentMesh = temp->mMeshes[0];
	for (int i = 0; i < currentMesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position.x = currentMesh->mVertices[i].x;
		vertex.position.y = currentMesh->mVertices[i].y;
		vertex.position.z = currentMesh->mVertices[i].z;
		if (currentMesh->HasNormals()) {
			vertex.normal.x = currentMesh->mNormals[i].x;
			vertex.normal.y = currentMesh->mNormals[i].y;
			vertex.normal.z = currentMesh->mNormals[i].z;
		}
		// check this to see if it really works
		if (currentMesh->HasTextureCoords(i)) {
			vertex.texCoordinates.x = currentMesh->mTextureCoords[i]->x;
			vertex.texCoordinates.y = currentMesh->mTextureCoords[i]->y;
		}
		ret->vertices.insert(ret->vertices.end(), vertex);
	}
	if (currentMesh->HasFaces()) {
		for (int i = 0; i < currentMesh->mNumFaces; i++) {
			ret->indices.insert(ret->indices.end(), currentMesh->mFaces[i].mIndices[0]);
			ret->indices.insert(ret->indices.end(), currentMesh->mFaces[i].mIndices[1]);
			ret->indices.insert(ret->indices.end(), currentMesh->mFaces[i].mIndices[2]);
		}
	}
	return ret;
}

Scene* WinAssetManager::loadScene(const char *path) {
	const char *extension = strchr(path, '.');
	if (strcmp(extension, ".level") == 0) {
		//Load a level
		PHYSFS_file* file = PHYSFS_openRead(path);
		if (file) {
			PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
			char* inBuff = new char[fileSize];
			PHYSFS_read(file, inBuff, fileSize, 1);

			//Budz fix za cudne karaktere koji se citaju iz fajla scene
			int i = fileSize - 1;
			for (; inBuff[i] != '>'; i--);
			inBuff[++i] = '\0';

			xml_document<> doc;
			doc.parse<0>(inBuff);
			xml_node<char>* mScene = doc.first_node("mscene");
			if (mScene)	{
				Scene* ret;
				//Get scene type and make a new scene object
				if (strcmp(mScene->first_attribute("type")->value(), "Level") == 0) {
					ret = new Level();
					ret->_name = mScene->first_attribute("name")->value();
				}

				for (xml_node<> *gameObject = mScene->first_node(); gameObject; gameObject = gameObject->next_sibling())
				{
					//Ensure it is a game object node
					if (strcmp(gameObject->name(), "gameObject") == 0) {
						//Get its type and call apropriate loader
						AbstractGameObjectLoader* loader = loaderFactory.getGameObjectLoader(gameObject->first_attribute("type")->value());
						ret->addGameObject(loader->load(gameObject));

					}
				}
				return ret;
			}
			return NULL;
		}
		else {
			return NULL;
		}
	}
	return NULL;
}


#endif