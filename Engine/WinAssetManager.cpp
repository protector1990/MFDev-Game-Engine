/** Copyright 2016 MarFil Studios. All rights reserved. **/

#define RAPIDXML_NO_EXCEPTIONS
#include "AssetManager.h"
#include <physfs.h>
#include <string>
#include <stdio.h>
#include "../include/assimp/Importer.hpp"
#include "../include/assimp/scene.h"
#include "Physfs2AssimpInterface.h"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "Sprite.h"
#include "Level.h"
#include "LoaderFactories.h"
#include "Common.h"

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
	PHYSFS_init(nullptr);
	const char *baseDir = PHYSFS_getBaseDir();
	char *dataPath = new char[strlen(baseDir) + 9];
	strcpy(dataPath, baseDir);
	strcat(dataPath, "data.zip");
	PHYSFS_mount(dataPath, "/", 1);
	char *uncompressedDataPath = new char[strlen(baseDir) + 5];
	strcpy(uncompressedDataPath, baseDir);
	strcat(uncompressedDataPath, "data");
	PHYSFS_mount(uncompressedDataPath, "/", 1);
	//We don't want Assimp to handle the file loading on its own. We want all loading to go through physfs
	asset3DImporter.SetIOHandler(new P2AIOSystem());
	//TODO: implement string pooling
	delete[] uncompressedDataPath;
	delete[] dataPath;
}

void WinAssetManager::deInit() {
	asset3DImporter.FreeScene();
	PHYSFS_deinit();
}

TextAsset* AssetManager::loadTextAsset(const char* path) {
	PHYSFS_file* file = PHYSFS_openRead(path);
	if (file)
	{
		PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
		char* inBuff = new char[fileSize + 1];
		PHYSFS_read(file, inBuff, fileSize, 1);
		inBuff[fileSize] = 0;

		TextAsset* ret = new TextAsset(const_cast<char*>(path), inBuff, fileSize);
		return ret;
	}
	else
	{
		return nullptr;
	}
}

int* WinAssetManager::loadInt(const char *path) {
	//implement this
	int i = 0;
	return &i;
}

SDL_Surface* WinAssetManager::loadSDL_Surface(const char *path) {
	PHYSFS_file* file = PHYSFS_openRead(path);
	if (file)
	{
		PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
		char* inBuff = new char[fileSize];
		PHYSFS_read(file, inBuff, fileSize, 1);
		SDL_RWops *rw = SDL_RWFromMem(inBuff, fileSize);
		return SDL_LoadBMP_RW(rw, 1);
	}
	else {
		return nullptr;
	}
}

Script* WinAssetManager::loadLuaScript(const char *path) {
	for (int i = 0; i < _loadedScripts.size(); i++) {
		if (strcmp(_loadedScripts[i]->_name, path) == 0) {
			return _loadedScripts[i];
		}
	}

	PHYSFS_file* file = PHYSFS_openRead(path);
	if (file) {
		PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
		char* inBuff = new char[fileSize + 1];
 		PHYSFS_read(file, inBuff, fileSize, 1);
		inBuff[fileSize] = 0;

		Script* ret = new Script(path, inBuff, fileSize);
		_loadedScripts.push_back(ret);
		//LuaManager::luaParseComponent(ENGINE.getLuaInterpreter(), ret);
		return ret;
	}
	else {
		return nullptr;
	}
}

ScriptClass* WinAssetManager::loadScriptClass(const char* path) {
	Script *script = loadLuaScript(path);
	ScriptClass *ret = new ScriptClass();
	// TODO: Implement error checking
	ret->setReference(SCRIPT_MANAGER->luaParseComponent(script));
	ret->setScript(script);
	return ret;
}

//TODO: make a system for "built-in" shaders
Shader* WinAssetManager::loadShader(const char* path) {
	for (Shader* shader : _loadedShaders)
	{
		if (!strcmp(path, shader->_name))
		{
			return shader;
		}
	}
	TextAsset* shaderText = loadTextAsset(path);
	Shader* ret = new Shader();
	const char* fileName = PHYSFS_getFileNameFromPath(path);
	// We introduce a shader naming convention:
	// All vertex shaders must begin with V, fragment shaders with F, and geometry shaders with G
	switch (*fileName)
	{
	case 'V': { // Vertex Shader
		ret->_type = VERTSHADER;
		ret->_shaderObject = glCreateShader(GL_VERTEX_SHADER);
		
	} break;
	case 'F': { // Fragment Shader
		ret->_type = FRAGSHADER;
		ret->_shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	} break;
	case 'G': { // Geometry Shader
		ret->_type = GEOMSHADER;
		ret->_shaderObject = glCreateShader(GL_GEOMETRY_SHADER);
	} break;
	default: delete ret; return nullptr;
	}
	glShaderSourceARB(ret->_shaderObject, 1, &shaderText->_contents, &shaderText->_size);
	glCompileShaderARB(ret->_shaderObject);

	GLint compiled;
	glGetObjectParameterivARB(ret->_shaderObject, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
#ifdef _DEBUG
		// get gl error
		char error[1024];
		glGetShaderInfoLog(ret->_shaderObject, 1024, nullptr, error);
		printf("[OpenGL shader]: Error with shader {%s}: %s", path, error);
#endif
		// next, cleanup memory and return null
		delete ret;
		return nullptr;
	}

	// Maybe keep it as Shader field?
	delete shaderText;
	ret->_name = new char[strlen(path) + 1];
	strcpy(ret->_name, path);
	_loadedShaders.push_back(ret);
	return ret;
}

// A more generic way of loading 3D assets. Will load whole scene
Asset3D* WinAssetManager::loadAsset3D(const char *path) {
	aiScene* temp = const_cast<aiScene*>(asset3DImporter.ReadFile(path, 0));
	Asset3D* ret = new Asset3D();
	ret->scene = temp;
	return ret;
}

// Will only load a single mesh. Intended for simple objects, not for complex scenes
// Finish this to be able to load vertex colors, skeletal animations and so on
Model3D* WinAssetManager::loadModel3D(const char *path) {
	const aiScene* temp = asset3DImporter.ReadFile(path, 0);
	if (!temp->HasMeshes()) {
		return nullptr;
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
			char* inBuff = new char[fileSize + 1];
			PHYSFS_read(file, inBuff, fileSize, 1);
			inBuff[fileSize] = 0;

			xml_document<> doc;
			doc.parse<0>(inBuff);
			xml_node<char>* mScene = doc.first_node("mscene");
			if (mScene)	{
				Scene* ret = nullptr;
				//Get scene type and make a new scene object
				if (strcmp(mScene->first_attribute("type")->value(), "Level") == 0) {
					ret = new Level();
					ret->_name = mScene->first_attribute("name")->value();
				}

				for (xml_node<> *gameObject = mScene->first_node(); gameObject; gameObject = gameObject->next_sibling())
				{
					//Ensure it is a game object node
					if (!strcmp(gameObject->name(), "gameObject")) {
						//Get its type and call apropriate loader
						AbstractGameObjectLoader* loader = loaderFactory.getGameObjectLoader(gameObject->first_attribute("type")->value());
						loader->_currentlyLoadingScene = ret;
						GameObject* obj = loader->load(gameObject);
						obj->_scene = ret;
						ret->addGameObject(obj);
					}
				}
				return ret;
			}
			return nullptr;
		}
		return nullptr;
	}
	return nullptr;
}


#endif