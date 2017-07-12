/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Common.h"
#include "SpriteLoader.h"
#include "Sprite.h"
#include "Engine.h"
#include <vector>
#include <mutex>

using namespace rapidxml;

//TODO: fix, fix, fix
namespace temp_sprite_loader {

	char* textureID = "texture";
	char* sheetID = "sheet";
	char* widthID = "width";
	char* heightID = "height";
	char* animSpeedID = "animSpeed";
	char* shader = "shader";
}

std::mutex spriteLoadingMutex;

GameObject* SpriteLoader::load(xml_node<char>* configuration) {
	spriteLoadingMutex.lock();

	Sprite* ret = new Sprite();
	_currentlyLoadingObject = ret;
	commonLoad(configuration);

	xml_node<char>* texture_node = configuration->first_node(temp_sprite_loader::textureID);
	char* texture_path = texture_node->value();
	ret->_texture = ASSET_MANAGER->loadAsset<MTexture>(texture_path);

	xml_node<char>* sheet = configuration->first_node(temp_sprite_loader::sheetID);
	xml_node<char>* sheetWidth = sheet->first_node(temp_sprite_loader::widthID);
	xml_node<char>* sheetHeight = sheet->first_node(temp_sprite_loader::widthID);
	xml_node<char>* animSpeed = sheet->first_node(temp_sprite_loader::animSpeedID);
	ret->_animSpeed = atof(animSpeed->value());
	ret->_sheetHeight = atoi(sheetWidth->value());
	ret->_sheetWidth = atoi(sheetHeight->value());
	if (xml_node<char>* shaderNode = configuration->first_node(temp_sprite_loader::shader))
	{
		Material* material = new Material();
		ShaderProgram shaderProgram;
		char shaderName[64];
		sprintf(shaderName, "/shaders/F%s.glsl", shaderNode->value());
		shaderProgram._frag = ASSET_MANAGER->loadAsset<Shader>(shaderName);
		sprintf(shaderName, "/shaders/V%s.glsl", shaderNode->value());
		shaderProgram._vert = ASSET_MANAGER->loadAsset<Shader>(shaderName);
		shaderProgram._id = glCreateProgram();
		glAttachShader(shaderProgram._id, shaderProgram._vert->_shaderObject);
		glAttachShader(shaderProgram._id, shaderProgram._frag->_shaderObject);
		if (shaderProgram._geom)
		{
			glAttachShader(shaderProgram._id, shaderProgram._geom->_shaderObject);
		}
		glLinkProgram(shaderProgram._id);

		GLint isLinked = 0;
		glGetProgramiv(shaderProgram._id, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			char error[1024];
			glGetProgramInfoLog(shaderProgram._id, 1024, nullptr, error);
			printf("[OpenGL shader]: Error with sprite shader linking: %s", error);
		}

		material->setShaderProgram(shaderProgram);
		ret->addMaterial(material);
	}
	ret->_components = loadComponents(configuration);
	ret->_scriptComponents = loadScriptComponents(configuration);
	spriteLoadingMutex.unlock();
	return static_cast<GameObject*>(ret);
}