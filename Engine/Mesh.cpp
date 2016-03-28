#include "Mesh.h"
#include <GL\glew.h>
#include <GL\wglew.h>
#include <SDL.h>
#include <SDL_opengl.h>

using namespace std;

mMesh::mMesh(vector<mVertex> vertices, vector<GLuint> indices, vector<mTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	this->setupMesh();
}

void mMesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(mVertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mVertex),
		(GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mVertex),
		(GLvoid*)offsetof(mVertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mVertex),
		(GLvoid*)offsetof(mVertex, TexCoords));

	glBindVertexArray(0);
}

void mMesh::Draw() {
	// Bind appropriate textures
	//GLuint diffuseNr = 1;
	//GLuint specularNr = 1;
	//for (GLuint i = 0; i < this->textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
	//	// Retrieve texture number (the N in diffuse_textureN)
	//	stringstream ss;
	//	string number;
	//	string name = this->textures[i].type;
	//	if (name == "texture_diffuse")
	//		ss << diffuseNr++; // Transfer GLuint to stream
	//	else if (name == "texture_specular")
	//		ss << specularNr++; // Transfer GLuint to stream
	//	number = ss.str();
	//	// Now set the sampler to the correct texture unit
	//	glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
	//	// And finally bind the texture
	//	glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	//}

	// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
	//glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);

	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	//for (GLuint i = 0; i < this->textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
}