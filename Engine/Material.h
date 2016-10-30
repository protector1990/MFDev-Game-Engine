/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#pragma once

#include "Texture.h"
#include "Shader.h"
#include <vector>
#include <tuple>
#include <map>
#include <glm/glm.hpp>

enum UniformTypes {
	UniformTypeFloat,
	UnoformTypeMat4x4,
	UniformTypeVec3,
	UniformTypeVec4,
	UniformTypeCount
};

class MaterialSystem {
public:
	static std::map<UniformTypes, size_t> uniformSizes;
	static void initMaterialSystem();

};

typedef std::pair<void*, UniformTypes> UniformPair;

struct Material {
private:
	ShaderProgram* _program;
	std::map<const char*, UniformPair> _uniforms;
	bool hasUniform(const char* uniformName);
public:
	Material();
	Material(const Material& other);
	// TODO: see how to solve uniform names
	void pushUniform(const char* name, void* data, UniformTypes type);
	void useMaterial();
};