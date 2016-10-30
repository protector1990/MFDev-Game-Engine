/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#include "Material.h"

std::map<UniformTypes, size_t> uniformSizes;

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

Material::Material(const Material& other) {
	_program = other._program;
	for (const UniformTuple& tuple : other._uniforms)
	{
		size_t size = uniformSizes[std::get<2>(tuple)];
		void* newMem = malloc(size);
		memcpy(newMem, std::get<1>(tuple), size);
		_uniforms.push_back(UniformTuple
			(
				std::get<0>(tuple),
				newMem,
				std::get<2>(tuple)
			));
	}
}

void Material::pushUniform(const char* name, void* data, UniformTypes type) {
	if (hasUniform(name))
	{
		// set uniform data to current value
	}

	size_t size = uniformSizes[type];
	void* newMem = malloc(size);
	memcpy(newMem, data, size);
	_uniforms.push_back(UniformTuple
	(
		name,
		newMem,
		type
	));
}

void Material::useMaterial() {
}
