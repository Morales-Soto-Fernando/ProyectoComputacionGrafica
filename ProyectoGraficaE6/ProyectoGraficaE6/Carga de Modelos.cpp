// Std. Includes
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

// Para imprimir el Working Directory en Windows (opcional pero útil)
#ifdef _WIN32
#include <direct.h>
#endif

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Utils
static bool fileExists(const std::string& p) { std::ifstream f(p); return f.good(); }
static void printWorkingDir() {
#ifdef _WIN32
    char cwd[2048];
    if (_getcwd(cwd, sizeof(cwd))) std::cout << "Working Directory: " << cwd << "\n";
#endif
}

// Camera
Camera  camera(glm::vec3(0.0f, 6.0f, 25.0f));
bool keys[1024]{};
GLfloat lastX = 400.0f, lastY = 300.0f;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW\n";
        return EXIT_FAILURE;
    }

    // GLFW config
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Grafica E6", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // opcional

    // GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW\n";
        return EXIT_FAILURE;
    }

    // Viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);                       // manejar canal alfa
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");

    // Imprime el WD para verificar rutas relativas
    printWorkingDir();

    // Ruta del OBJ (OBJ/MTL/JPG en la MISMA carpeta .../Models/Pasto/)
    const std::string objPath = "Models/Calle/Pasto.obj";
    if (!fileExists(objPath)) {
        std::cout << "ERROR: No existe el archivo OBJ en: " << objPath << "\n"
            << "Asegura que el Working Directory sea la carpeta del proyecto (Props → Debugging → Working Directory = $(ProjectDir)).\n";
        return EXIT_FAILURE;
    }

    // Cargar modelo
    Model dog((char*)objPath.c_str());

    // Matrices
    glm::mat4 projection = glm::perspective(camera.GetZoom(),
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Timing
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        glfwPollEvents();
        DoMovement();

        // Clear
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw
        shader.Use();
        glm::mat4 view = camera.GetViewMatrix();

        GLint projLoc = glGetUniformLocation(shader.Program, "projection");
        GLint viewLoc = glGetUniformLocation(shader.Program, "view");
        GLint modelLoc = glGetUniformLocation(shader.Program, "model");

        if (projLoc != -1) glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        if (viewLoc != -1) glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Perro (Pasto.obj en tu caso)
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 5.5f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

        // Swap
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos; // invertido porque Y de pantalla crece hacia abajo

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
