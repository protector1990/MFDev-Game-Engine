/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#pragma once

#include "Texture.h"
#include "Shader.h"
#include <vector>
#include <tuple>
#include <map>
#include <glm/glm.hpp>
#include <SDL_opengl.h>

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

typedef std::pair<void*, UniformTypes> UniformValuePair;
typedef std::pair<const char*, UniformValuePair> UniformPair;

struct Material {
private:
	ShaderProgram* _program;
	std::map<const char*, GLfloat> _uniformFloats;
	std::map<const char*, glm::vec3> _uniformVec3s;
	std::map<const char*, glm::vec4> _uniformVec4s;
	std::map<const char*, glm::mat4> _uniformMat4s;
	std::map<const char*, GLint> _uniformSamplers2D;
	bool hasUniform(const char* uniformName);
public:
	Material();
	Material(const Material& other);
	// TODO: see how to solve uniform names
	void pushUniformFloat(const char* name, float data);
	void pushUniformVec3(const char* name, glm::vec3 data);
	void pushUniformVec4(const char* name, glm::vec4 data);
	void pushUniformMat4(const char* name, glm::mat4 data);
	void pushUniformSampler2D(const char* name, GLint data);
	void useMaterial();
	inline void setShaderProgram(ShaderProgram* program);
};