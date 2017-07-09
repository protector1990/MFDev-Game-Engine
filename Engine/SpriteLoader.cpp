/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Common.h"
#include "SpriteLoader.h"
#include "Sprite.h"
#include "Engine.h"
#include <vector>

using namespace rapidxml;

//TODO: fix, fix, fix
namespace temp_sprite_loader {

	char* transformID = "transform";
	char* positionXID = "positionX";
	char* positionYID = "positionY";
	char* positionZID = "positionZ";

	char* scaleXID = "scaleX";
	char* scaleYID = "scaleY";
	char* scaleZID = "scaleZ";

	char* textureID = "texture";
	char* sheetID = "sheet";
	char* widthID = "width";
	char* heightID = "height";
	char* animSpeedID = "animSpeed";
	char* parent = "parent";
	char* shader = "shader";
}

GameObject* SpriteLoader::load(xml_node<char>* configuration) {
	Sprite* ret = new Sprite();

	const char* tempName = configuration->first_attribute("name")->value();
	ret->_name = new char[strlen(tempName) + 1];
	strcpy(ret->_name, tempName);

	// See where this line will fit best
	_currentlyLoadingObject = ret;


	xml_node<char>* texture_node = configuration->first_node(temp_sprite_loader::textureID);
	char* texture_path = texture_node->value();
	ret->_texture = ASSET_MANAGER->loadAsset<MTexture>(texture_path);

	xml_node<char>* transform = configuration->first_node(temp_sprite_loader::transformID);
	xml_node<char>* positionX = transform->first_node(temp_sprite_loader::positionXID);
	xml_node<char>* positionY = transform->first_node(temp_sprite_loader::positionYID);
	xml_node<char>* positionZ = transform->first_node(temp_sprite_loader::positionZID);

	xml_node<char>* scaleX = transform->first_node(temp_sprite_loader::scaleXID);
	xml_node<char>* scaleY = transform->first_node(temp_sprite_loader::scaleYID);
	xml_node<char>* scaleZ = transform->first_node(temp_sprite_loader::scaleZID);
	ret->_transform.translate(glm::vec3(atof(positionX->value()), atof(positionY->value()), atof(positionZ->value())));
	ret->_transform.setScale(glm::vec3(atof(scaleX->value()), atof(scaleY->value()), atof(scaleZ->value())));
	xml_node<char>* parent = transform->first_node(temp_sprite_loader::parent);
	if (parent)
	{
		ret->setParent(_currentlyLoadingScene->getGameObjectByName(parent->value()));
	}
	//ret->_Position.x = atof(positionX->value());
	//ret->_Position.y = atof(positionY->value());
	//ret->_Position.z = atof(positionZ->value());

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

	return static_cast<GameObject*>(ret);
}