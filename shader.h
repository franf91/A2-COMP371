#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Hardcoded vertex shader sources
extern const char* vertexShaderSource;
extern const char* fragmentShaderSource;

class Shader
{
public:
	// Program ID
	GLuint ID;

	// Constructor
	Shader();

	// Use/activate the shader
	void use() const;

	// Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setMat4(const std::string& name, const glm::mat4& matrix) const;

private:
	// Utility function for checking shader compilation/linking errors
	void checkCompileErrors(GLuint shader, std::string type);
};