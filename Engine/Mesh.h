/**  Copyright 2016 MarFil Studios  **/
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include <vector>
#include <..\include\assimp\types.h>


struct mTexture {
	GLuint id;
	std::string type;
	aiString path;
};

struct mVertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
};

class mMesh {
public:
	mMesh(std::vector<mVertex> vertices, std::vector<GLuint> indices, std::vector<mTexture> textures);
	mMesh();
	/*  Mesh Data  */
	std::vector<mVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<mTexture> textures;
	/*  Functions  */
	mMesh(std::vector<mVertex> vertices, std::vector<GLuint> indices, std::vector<mTexture> textures);
	void Draw();
private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};