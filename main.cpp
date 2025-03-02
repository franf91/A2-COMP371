#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"

"out vec3 vertexColor;"

"uniform mat4 model;"
"uniform mat4 view;"
"uniform mat4 proj;"

"void main()\n"
"{\n"
"   gl_Position = proj * view * model * vec4(aPos, 1.0);\n"
"   vertexColor = aColor;\n"
"}\n\0";

// Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 vertexColor;\n"
"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor, 1.0f);\n"
"}\n\0";

// Global variables for transformations
glm::vec3 translation = glm::vec3(0.0f);
float rotationAngle = 0.0f;
float scaleFactor = 1.0f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Process input function
void processInput(GLFWwindow* window)
{
    float d = 0.1f; // translation step
    float s = 1.05f; // scaling factor

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        translation.y += d;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        translation.y -= d;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        translation.x -= d;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        translation.x += d;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        scaleFactor *= s;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        scaleFactor /= s;
}

// The rest of the code remains the same, applying the transformations in the render loop

int main()
{
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

    //keycallback
    glfwSetKeyCallback(window, keyCallback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    // Create Vertex Shader Object and get its reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compile the Vertex Shader into machine code
    glCompileShader(vertexShader);

    // Create Fragment Shader Object and get its reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compile the Vertex Shader into machine code
    glCompileShader(fragmentShader);

    // Create Shader Program Object and get its reference
    GLuint shaderProgram = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(shaderProgram);

    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertices coordinates
    GLfloat vertices[] =
    {//     COORDINATES     /        COLORS       //
     -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f,
     -0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f,
     0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4 };

    // Create reference containers for the Vartex Array Object, the Vertex Buffer Object, and the Element Buffer Object
    GLuint VAO, VBO, EBO;

    // Generate the VAO, VBO, and EBO with only 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Make the VAO the current Vertex Array Object by binding it
    glBindVertexArray(VAO);

    // Bind the VBO specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the EBO specifying it's a GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Introduce the indices into the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //  Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // Bind the EBO to 0 so that we don't accidentally modify it
    // MAKE SURE TO UNBIND IT AFTER UNBINDING THE VAO, as the EBO is linked in the VAO
    // This does not apply to the VBO because the VBO is already linked to the VAO during glVertexAttribPointer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        glUseProgram(shaderProgram);

        processInput(window);

        // Initialize matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -10.0f));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)800 / 800, 0.1f, 100.0f);

        // Apply transformations
        model = glm::translate(model, translation);
        model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(scaleFactor));

        // Set uniforms and draw the pyramid
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(shaderProgram, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup code
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//process input (for single key press)
void keyCallback(GLFWwindow* window, int key, int sscancode, int action, int mods) {

    //rotate right
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        rotationAngle -= glm::radians(30.0f);
    }
    //rotate left
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        rotationAngle += glm::radians(30.0f);
    }
}
