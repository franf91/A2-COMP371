#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputHandler
{
public:
	InputHandler();

	void setupCallbacks(GLFWwindow* window);
	void processInput(GLFWwindow* window);

	// Getters for transformation values
	glm::vec3 getTranslation() const { return translation; }
	float getRotationAngle() const { return rotationAngle; }
	float getScaleFactor() const { return scaleFactor; }

	// Called by the static callback function
	void handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	// Transformation state
	glm::vec3 translation;
	float rotationAngle;
	float scaleFactor;

	// Static callback function that will call the member function
	static void keyCallbackDispatcher(GLFWwindow* window, int key, int scancode, int action, int mods);
};
