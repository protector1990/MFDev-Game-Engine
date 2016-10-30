/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Model3D.h"

using namespace glm;

void Model3D::transform(mat4x4 transformMatrix) {

}

void Model3D::setUp() {
	glGenVertexArrays(1, &VertexArrayObjects);
	glGenBuffers(1, &VertexBufferObjects);
	glGenBuffers(1, &ElementBufferObjects);
	glBindVertexArray(VertexArrayObjects);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjects);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObjects);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex3D), nullptr);

	// Vertex3D Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
		reinterpret_cast<GLvoid*>(offsetof(Vertex3D, normal)));
	//glNormalPointer(GL_FLOAT, sizeof(Vertex3D), (GLvoid*)offsetof(Vertex3D, normal));
	// Vertex3D Texture Coords
	//glEnableVertexAttribArray(2);7glNormalPointer(GL_FLOAT, sizeof(Vertex3D), (GLvoid*)&(vertices[0].normal));
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(Vertex3D), (GLvoid*)offsetof(Vertex3D, normal));
	glBindVertexArray(0);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
	//	(GLvoid*)offsetof(Vertex3D, TexCoords));
}

void Model3D::draw() {
	glBindVertexArray(VertexArrayObjects);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
	//	(GLvoid*)offsetof(Vertex3D, normal));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}