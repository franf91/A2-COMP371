#include "pyramid.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "shader.h"

Pyramid::Pyramid()
{
	// Initialize vertices with coordinates and colors
	GLfloat verts[] = {
		//     COORDINATES     /        COLORS       //
		-0.5f, 0.0f, 0.5f,     0.83f, 0.70f, 0.44f,  // 0 - front left
		-0.5f, 0.0f, -0.5f,    1.0f, 0.0f, 0.0f,     // 1 - back left
		0.5f, 0.0f, -0.5f,     1.0f, 0.0f, 0.0f,     // 2 - back right
		0.5f, 0.0f, 0.5f,      0.83f, 0.70f, 0.44f,  // 3 - front right
		0.0f, 0.8f, 0.0f,      0.92f, 0.86f, 0.76f   // 4 - top
	};

	std::memcpy(vertices, verts, sizeof(verts));

	// Initialize indices for triangular faces
	GLuint inds[] = {
		0, 1, 2,  // Base - triangle 1
		0, 2, 3,  // Base - triangle 2
		0, 1, 4,  // Left side
		1, 2, 4,  // Back side
		2, 3, 4,  // Right side
		3, 0, 4   // Front side
	};

	std::memcpy(indices, inds, sizeof(inds));
}

Pyramid::~Pyramid()
{
}

void Pyramid::initialize()
{
	// Generate VAO, VBO, and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind and fill VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind and fill EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set vertex attribute pointers
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind to prevent accidental modification
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Pyramid::draw(Shader& shader, const glm::vec3& translation, float rotationAngle, float scaleFactor) const
{
	// Transformation for the pyramid
	glm::mat4 model = glm::mat4(1.0f);

	// Apply transformations in order: translate, rotate, scale
	model = glm::translate(model, translation);
	model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, scaleFactor));

	// Set model matrix uniform
	shader.setMat4("model", model);

	// Bind the VAO and draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Pyramid::cleanup() const
{
	// Delete buffers and vertex array
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}