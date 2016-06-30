/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <SDL_opengl.h>
#include "Texture.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoordinates;
	Texture* texture;
};

//Represents a single mesh
struct Model3D {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint VertexArrayObjects;
	GLuint VertexBufferObjects;
	GLuint ElementBufferObjects;
	void transform(glm::mat4x4 transformMatrix);
	void setUp();
	void draw();
};