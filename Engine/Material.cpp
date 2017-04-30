/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#include <gl/glew.h>
#include "Material.h"


std::map<UniformTypes, size_t> MaterialSystem::uniformSizes;
void MaterialSystem::initMaterialSystem() {
	uniformSizes[UniformTypeFloat] = sizeof(GLfloat);
	uniformSizes[UnoformTypeMat4x4] = sizeof(glm::mat4);
	uniformSizes[UniformTypeVec3] = sizeof(glm::vec3);
	uniformSizes[UniformTypeVec4] = sizeof(glm::vec4);
}

//TODO: implement
bool Material::hasUniform(const char* uniformName) {
	return false;
}

Material::Material() {
}

void Material::pushUniformFloat(const char* name, float data) {
	if (_uniformFloats.find(name) != _uniformFloats.end())
	{
		_uniformFloats[name] = data;
	}
	else
	{
		_uniformFloats.insert(std::make_pair(name, data));
	}
}

void Material::pushUniformVec3(const char* name, glm::vec3 data) {
	if (_uniformVec3s.find(name) != _uniformVec3s.end())
	{
		_uniformVec3s[name] = data;
	}
	else
	{
		_uniformVec3s.insert(std::make_pair(name, data));
	}
}

void Material::pushUniformVec4(const char* name, glm::vec4 data) {
	if (_uniformVec4s.find(name) != _uniformVec4s.end())
	{
		_uniformVec4s[name] = data;
	}
	else
	{
		_uniformVec4s.insert(std::make_pair(name, data));
	}
}

void Material::pushUniformMat4(const char* name, glm::mat4 data) {
	if (_uniformMat4s.find(name) != _uniformMat4s.end())
	{
		_uniformMat4s[name] = data;
	}
	else
	{
		_uniformMat4s.insert(std::make_pair(name, data));
	}
}

void Material::useMaterial() {
	glUseProgram(_program->_id);
	for (const std::pair<const char*, GLfloat>& pair : _uniformFloats)
	{
		GLuint location = glGetUniformLocation(_program->_id, pair.first);
		glUniform1f(location, pair.second);
	}
	for (const std::pair<const char*, glm::vec3>& pair : _uniformVec3s)
	{
		GLuint location = glGetUniformLocation(_program->_id, pair.first);
		glUniform3f(location, pair.second.x, pair.second.y, pair.second.z);
	}
	for (const std::pair<const char*, glm::vec4>& pair : _uniformVec4s)
	{
		GLuint location = glGetUniformLocation(_program->_id, pair.first);
		glUniform4f(location, pair.second.x, pair.second.y, pair.second.z, pair.second.w);
	}
	for (const std::pair<const char*, glm::mat4>& pair : _uniformMat4s)
	{
		GLuint location = glGetUniformLocation(_program->_id, pair.first);
		glUniformMatrix4fv(location, 1, GL_FALSE, &pair.second[0][0]);
	}
}

void Material::setShaderProgram(ShaderProgram* program) {
	_program = program;
}
