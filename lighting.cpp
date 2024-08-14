#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Vertex Shader Source
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

// Fragment Shader Source
const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";

// Function declarations
unsigned int createShaderProgram();
void setupVertexData(unsigned int &VAO, unsigned int &VBO);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create GLFW window
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Basic Primitives with Lighting", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Create and compile shaders
    unsigned int shaderProgram = createShaderProgram();

    // Set up vertex data
    unsigned int VBO, VAO;
    setupVertexData(VAO, VBO);

    // Define vertices for a cube
    float vertices[] = {
        // Positions          // Normals
        -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, -1.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 1.0f, -1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, -1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, -1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 1.0f, 1.0f};

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // Front face
        4, 5, 6, 6, 7, 4, // Back face
        4, 5, 1, 1, 0, 4, // Bottom face
        7, 6, 2, 2, 3, 7, // Top face
        4, 7, 3, 3, 0, 4, // Left face
        1, 5, 6, 6, 2, 1  // Right face
    };

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        // Set up transformations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Set lighting properties
        unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
        unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        unsigned int objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");

        glUniform3f(lightPosLoc, 1.0f, 1.0f, 1.0f);     // Light position
        glUniform3f(viewPosLoc, 0.0f, 0.0f, 3.0f);      // Camera/view position
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);   // White light
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f); // Object color

        // Bind Vertex Array Object
        glBindVertexArray(VAO);

        // Draw cube
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

unsigned int createShaderProgram()
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // Clean up shaders as they're linked into the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void setupVertexData(unsigned int &VAO, unsigned int &VBO)
{
    float vertices[] = {
        // Positions          // Normals
        -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, -1.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 1.0f, -1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, -1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, -1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 1.0f, 1.0f};

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // Front face
        4, 5, 6, 6, 7, 4, // Back face
        4, 5, 1, 1, 0, 4, // Bottom face
        7, 6, 2, 2, 3, 7, // Top face
        4, 7, 3, 3, 0, 4, // Left face
        1, 5, 6, 6, 2, 1  // Right face
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}