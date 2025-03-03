#include "shader.h"
#include "pyramid.h"
#include "input_handler.h"
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return -1;
	}

	// Specify OpenGL version and profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Required for MacOS
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Pyramid with EBO", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the OpenGL context current before initializing GLEW
	glfwMakeContextCurrent(window);

	// Set up input handling
	InputHandler inputHandler;
	inputHandler.setupCallbacks(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW!" << std::endl;
		return -1;
	}

	// Create and compile shaders
	Shader shader;

	// Create pyramid object
	Pyramid pyramid;
	pyramid.initialize();

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Main render loop
	double lastTime = glfwGetTime();
	const double targetFrameTime = 1.0 / 60.0; // Run at 60 FPS
	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		// Sleep until we reach the next frame time
		if (deltaTime < targetFrameTime)
		{
			double sleepTime = targetFrameTime - deltaTime;
			std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
			continue;
		}
		lastTime = glfwGetTime();

		// Background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Process input
		inputHandler.processInput(window);

		// Use our shader program
		shader.use();

		// Set view and projection matrices
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -10.0f));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("proj", projection);

		// Draw the pyramid
		pyramid.draw(shader, inputHandler.getTranslation(),
			inputHandler.getRotationAngle(),
			inputHandler.getScaleFactor());

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup on exit
	pyramid.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
