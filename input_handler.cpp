#include "input_handler.h"
#include <glm/gtc/matrix_transform.hpp>

// Used to store the input handler instance for the static callback
static InputHandler* currentHandler = nullptr;

InputHandler::InputHandler()
	: translation(0.0f, 0.0f, 0.0f), rotationAngle(0.0f), scaleFactor(1.0f)
{
	// Store the instance for use in static callback
	currentHandler = this;
}

void InputHandler::setupCallbacks(GLFWwindow* window)
{
	// Set the key callback
	glfwSetKeyCallback(window, keyCallbackDispatcher);
}

// Static callback function that dispatches to the instance method
void InputHandler::keyCallbackDispatcher(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (currentHandler)
	{
		currentHandler->handleKeyPress(window, key, scancode, action, mods);
	}
}

// Used to handle single key presses not continuous
void InputHandler::handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Rotation controls
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
			rotationAngle += glm::radians(30.0f);
			break;
		case GLFW_KEY_E:
			rotationAngle -= glm::radians(30.0f);
			break;
		}
	}
}

void InputHandler::processInput(GLFWwindow* window)
{
	// Constants for input sensitivity
	float translationSpeed = 0.1f;
	float scaleSpeed = 0.1f;

	// Handle continuous key presses (WASD for movement)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		translation.y += translationSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		translation.y -= translationSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		translation.x -= translationSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		translation.x += translationSpeed;

	// Handle scaling
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		scaleFactor += scaleSpeed;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		scaleFactor -= scaleSpeed;

	// Handle escape key to close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}