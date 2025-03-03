#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;

class Pyramid
{
public:
	Pyramid();
	~Pyramid();

	void initialize();
	void draw(Shader& shader, const glm::vec3& translation, float rotationAngle, float scaleFactor) const;
	void cleanup() const;

private:
	GLuint VAO, VBO, EBO;
	GLfloat vertices[30];  // 5 vertices * 6 floats each (x,y,z,r,g,b)
	GLuint indices[18];    // 6 faces * 3 vertices each
};
