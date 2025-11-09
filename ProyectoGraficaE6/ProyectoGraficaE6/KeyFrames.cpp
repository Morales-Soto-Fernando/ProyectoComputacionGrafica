/*
Proyecto Equipo 6
Fecha de entrega 12 de Noviembre del 2025
315143977
318063188
319279207
 */
#include <iostream>
#include <cmath>
#include <fstream>


// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"



// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(27.0f, 4.0f, 68.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
float rotDogX = 0; //Variable para rotación
int dogAnim = 0;
float FLegs = 0.0f;
float FLegsL = 0.0f;
float FLegsR = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;

//float radio = 0.0f;  // Radio del círculo
float angle = 0.0f;  // Ángulo inicial
bool moveRot = false;
float speed = 0.0001f;

// Variables globales del murciélago
float batPosX = 17.3f;
float batPosY = 14.5f;
float batPosZ = 35.0f;
float batAngle = 0.0f;
float wingAngle = 0.0f;
bool wingUp = true;
float tailSwing = 0.5f;
int batDirection = 1;

// Variables globales del segundo murciélago
float bat2PosX = 10.0f;
float bat2PosY = 20.0f;
float bat2PosZ = 5.0f;
float bat2Angle = 0.0f;
float wingAngle2 = 0.0f;
bool wingUp2 = true;



// ---- Persona (variables propias) ----
float perPosX = 0.0f, perPosY = 0.0f, perPosZ = 0.0f;  // posición de la persona
float perRotY = 0.0f, perRotX = 0.0f;                  // yaw/pitch del torso
float perHead = 0.0f;   // cabeza
float perArmL = 0.0f;   // hombro izquierdo
float perArmR = 0.0f;   // hombro derecho
float perLegL = 0.0f;   // muslo izquierdo
float perLegR = 0.0f;   // muslo derecho

//KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 9
int i_max_steps = 500;
int i_curr_steps = 0; //En qué punto de la línea de tiempo nos encontramos
typedef struct _frame {

	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float head;
	float headInc;
	float tail;
	float tailInc;
	float FLegs;
	float FLegsInc;
	float FLegsL;
	float FLegsLInc;
	float FLegsR;
	float FLegsRInc;
	float RLegs;
	float RLegsInc;
	//Variables para la rotación
	float rotDogX;
	float rotDogXInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Función para guardar la animación
void SaveAnimation(const char* filename = "Animacion.txt") {
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo para guardar." << std::endl;
		return;
	}

	for (int i = 0; i < FrameIndex; i++) {
		file << KeyFrame[i].dogPosX << " "
			<< KeyFrame[i].dogPosY << " "
			<< KeyFrame[i].dogPosZ << " "
			<< KeyFrame[i].rotDog << " "
			<< KeyFrame[i].head << " "
			<< KeyFrame[i].tail << " "
			<< KeyFrame[i].FLegs << " "
			<< KeyFrame[i].FLegsL << " "
			<< KeyFrame[i].FLegsR << " "
			<< KeyFrame[i].RLegs << " "
			<< KeyFrame[i].rotDogX << "\n";
	}
	file.close();
	std::cout << "Animacion guardada correctamente." << std::endl;
}

// Función para cargar los KeyFrames
void LoadAnimation(const char* filename = "Animacion.txt") {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo para cargar." << std::endl;
		return;
	}

	FrameIndex = 0;
	while (FrameIndex < MAX_FRAMES &&
		file >> KeyFrame[FrameIndex].dogPosX
		>> KeyFrame[FrameIndex].dogPosY
		>> KeyFrame[FrameIndex].dogPosZ
		>> KeyFrame[FrameIndex].rotDog
		>> KeyFrame[FrameIndex].head
		>> KeyFrame[FrameIndex].tail
		>> KeyFrame[FrameIndex].FLegs
		>> KeyFrame[FrameIndex].FLegsL
		>> KeyFrame[FrameIndex].FLegsR
		>> KeyFrame[FrameIndex].RLegs
		>> KeyFrame[FrameIndex].rotDogX) {
		FrameIndex++;
	}
}

// Función para imprimir el contenido del archivo .txt en consola
void PrintAnimation(const char* filename = "Animacion.txt") {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo para imprimir." << std::endl;
		return;
	}

	std::string line;
	std::cout << "Contenido del archivo " << filename << ":\n";
	while (std::getline(file, line)) {
		std::cout << line << std::endl;
	}
	file.close();
}

void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);
	KeyFrame[FrameIndex].dogPosX = perPosX;
	KeyFrame[FrameIndex].dogPosY = perPosY;
	KeyFrame[FrameIndex].dogPosZ = perPosZ;

	KeyFrame[FrameIndex].rotDog = perRotY;   // yaw torso
	KeyFrame[FrameIndex].rotDogX = perRotX;   // pitch torso

	KeyFrame[FrameIndex].head = perHead;
	KeyFrame[FrameIndex].tail = 0.0f;       // no usado en persona
	KeyFrame[FrameIndex].FLegs = 0.0f;       // no usado (o úsalo si quieres)
	KeyFrame[FrameIndex].FLegsL = perArmL;
	KeyFrame[FrameIndex].FLegsR = perArmR;
	KeyFrame[FrameIndex].RLegs = 0.0f;       // puedes mapear a piernas si lo deseas

	FrameIndex++;

	

	//KeyFrame[FrameIndex].dogPosX = dogPosX;
	//KeyFrame[FrameIndex].dogPosY = dogPosY;
	//KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	//KeyFrame[FrameIndex].rotDog = rotDog;
	//KeyFrame[FrameIndex].rotDogX = rotDogX;

	//KeyFrame[FrameIndex].head = head;
	//KeyFrame[FrameIndex].tail = tail;
	//KeyFrame[FrameIndex].FLegs = FLegs;
	//KeyFrame[FrameIndex].FLegsL = FLegsL;
	//KeyFrame[FrameIndex].FLegsR = FLegsR;
	//KeyFrame[FrameIndex].RLegs = RLegs;

	//FrameIndex++;
}

void resetElements(void)
{
	perPosX = KeyFrame[0].dogPosX;
	perPosY = KeyFrame[0].dogPosY;
	perPosZ = KeyFrame[0].dogPosZ;

	perHead = KeyFrame[0].head;
	perArmL = KeyFrame[0].FLegsL;
	perArmR = KeyFrame[0].FLegsR;
	perLegL = 0.0f;
	perLegR = 0.0f;

	perRotY = KeyFrame[0].rotDog;
	perRotX = KeyFrame[0].rotDogX;

	//dogPosX = KeyFrame[0].dogPosX;
	//dogPosY = KeyFrame[0].dogPosY;
	//dogPosZ = KeyFrame[0].dogPosZ;

	//head = KeyFrame[0].head;
	//tail = KeyFrame[0].tail;
	//FLegs = KeyFrame[0].FLegs;
	//FLegsL = KeyFrame[0].FLegsL;
	//FLegsR = KeyFrame[0].FLegsR;
	//RLegs = KeyFrame[0].RLegs;

	//rotDog = KeyFrame[0].rotDog;
	//rotDogX = KeyFrame[0].rotDogX;

}
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;

	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
	KeyFrame[playIndex].FLegsInc = (KeyFrame[playIndex + 1].FLegs - KeyFrame[playIndex].FLegs) / i_max_steps;
	KeyFrame[playIndex].FLegsLInc = (KeyFrame[playIndex + 1].FLegsL - KeyFrame[playIndex].FLegsL) / i_max_steps;
	KeyFrame[playIndex].FLegsRInc = (KeyFrame[playIndex + 1].FLegsR - KeyFrame[playIndex].FLegsR) / i_max_steps;
	KeyFrame[playIndex].RLegsInc = (KeyFrame[playIndex + 1].RLegs - KeyFrame[playIndex].RLegs) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
	KeyFrame[playIndex].rotDogXInc = (KeyFrame[playIndex + 1].rotDogX - KeyFrame[playIndex].rotDogX) / i_max_steps;

	printf("Interpolando cuadro %d a %d: IncX = %f\n", playIndex, playIndex + 1, KeyFrame[playIndex].incX);
}



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/


	// Create a GLFWwindow object that we can use for GLFW's functions

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Equipo 6", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");


	//models
	Model Galeria((char*)"Models/Galeria/Galeria4.obj");
	Model Piso((char*)"Models/piso/Piso.obj");
	Model Lampara((char*)"Models/Exterior/streetlamp.obj");
	Model Arbol((char*)"Models/Exterior/10445_Oak_Tree_v1_max2010_iteration-1.obj");
	Model Hands((char*)"Models/Interior/Portal.obj");
	Model Demon((char*)"Models/Interior/Demon.obj");
	Model Witch((char*)"Models/Interior/witch2.obj");
	Model Ritual((char*)"Models/Interior/ritual.obj");
	Model Corpse((char*)"Models/Interior/corpse.obj");

	// Perro
	Model PerroTor((char*)"Models/Exterior/PeroCortado2/PerroTorso.obj");
	Model PerroCab((char*)"Models/Exterior/PeroCortado2/PerroCabeza.obj");
	Model PerroCol((char*)"Models/Exterior/PeroCortado2/Cola.obj");
	Model PerroPatFD((char*)"Models/Exterior/PeroCortado2/PataFroder.obj");
	Model PerroPatFI((char*)"Models/Exterior/PeroCortado2/PataFroizq.obj");
	Model PerroPatTD((char*)"Models/Exterior/PeroCortado2/PataTrader.obj");
	Model PerroPatTI((char*)"Models/Exterior/PeroCortado2/PataTraizq.obj");


	//Murcielago
	Model BatAguja((char*)"Models/Exterior/Murcielago/Aguja.obj");
	Model BatAlader((char*)"Models/Exterior/Murcielago/Alader.obj");
	Model BatAlaizq((char*)"Models/Exterior/Murcielago/Alaizq.obj");
	Model BatCuerpo((char*)"Models/Exterior/Murcielago/Torsomurci.obj");


	Model Sofa((char*)"Models/Interior/sofa.obj");
	Model Recepcion((char*)"Models/Interior/recepcion.obj");
	Model Mesa((char*)"Models/Exterior/picnic_table.obj");

	// Persona (partes) 
	Model Ptorso((char*)"Models/Galeria/Persona/Ptorso.obj");
	Model Pcabeza((char*)"Models/Galeria/Persona/Pcabeza.obj");
	Model Phombroizq((char*)"Models/Galeria/Persona/Phombroizq.obj");
	Model Phombroder((char*)"Models/Galeria/Persona/Phombroder.obj");
	Model Pantebrasizq((char*)"Models/Galeria/Persona/Pantebrasizq.obj");
	Model Pantebrasder((char*)"Models/Galeria/Persona/Pantebrasder.obj");
	Model Pmusloizq((char*)"Models/Galeria/Persona/Pmusloizq.obj");
	Model Pmusloder((char*)"Models/Galeria/Persona/Pmusloder.obj");
	Model Prodillaizq((char*)"Models/Galeria/Persona/Prodillaizq.obj");
	Model Prodillader((char*)"Models/Galeria/Persona/Prodillader.obj");




	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
		KeyFrame[i].FLegs = 0;
		KeyFrame[i].FLegsInc = 0;
		KeyFrame[i].FLegsL = 0;
		KeyFrame[i].FLegsLInc = 0;
		KeyFrame[i].FLegsR = 0;
		KeyFrame[i].FLegsRInc = 0;
		KeyFrame[i].RLegs = 0;
		KeyFrame[i].RLegsInc = 0;
		KeyFrame[i].rotDogX = 0;
		KeyFrame[i].rotDogXInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
	}

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	///////////////////BUFFER SKYBOX//////////////////
	glGenBuffers(1, &EBO);
	/////////////////////////////////////////////////
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	///////////////////PIPELINE RENDERIZADO SKYBOX//////////////////
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	///////////////////////////////////////////////////////////////


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 1);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	///////////////////////////SKYBOX/////////////////////////
	GLuint skyBoxVBO, skyBoxVAO;
	glGenVertexArrays(1, &skyBoxVAO);
	glGenBuffers(1, &skyBoxVBO);
	glBindVertexArray(skyBoxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Load textures
	vector < const GLchar*> faces;
	faces.push_back("SkyBox/pz.jpg");
	faces.push_back("SkyBox/nz.jpg");
	faces.push_back("SkyBox/py.jpg");
	faces.push_back("SkyBox/ny.jpg");
	faces.push_back("SkyBox/nx.jpg");
	faces.push_back("SkyBox/px.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	/////////////////////////////////////////////////////////

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();
		

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		// --- ANIMACIÓN DEL PRIMER MURCIÉLAGO 
		float tiempo = glfwGetTime();

		// Radio de la circunferencia del vuelo
		float radio = 25.5f;
		float velocidadVuelo = 0.5f;
		batPosX = cos(tiempo * velocidadVuelo) * radio + 15.0f;  // centrado donde antes estaba
		batPosZ = sin(tiempo * velocidadVuelo) * radio + 40.0f;
		batPosY = 14.0f + sin(tiempo * 2.0f) * 12.5f; // pequeño aleteo vertical

		batAngle = -glm::degrees(tiempo * velocidadVuelo) - 90.0f; // rotar en dirección del vuelo

		// Velocidad de movimiento 
		float velocidadAla = 0.09f;
		if (wingUp)
			wingAngle += velocidadAla;
		else
			wingAngle -= velocidadAla;

		// Apertura de las alas
		if (wingAngle > 1.5f) wingUp = false;
		if (wingAngle < -1.5f) wingUp = true;

		// Movimiento de la cola
		tailSwing = sin(tiempo * 3.0f) * 15.0f;


		// --- ANIMACIÓN DEL SEGUNDO MURCIÉLAGO
		float tiempo2 = glfwGetTime();

		// Radio de la circunferencia del vuelo
		float radio2 = 10.0f;
		float velocidadVuelo2 = 0.5f;
		bat2PosX = cos(tiempo2 * velocidadVuelo2) * radio2 + 15.0f;  // centrado donde antes estaba
		bat2PosZ = sin(tiempo2 * velocidadVuelo2) * radio2 + 40.0f;
		bat2PosY = 21.0f + sin(tiempo2 * 2.0f) * 12.5f; // pequeño aleteo vertical

		bat2Angle = -glm::degrees(tiempo2 * velocidadVuelo2) - 90.0f; // rotar en dirección del vuelo

		// Velocidad de movimiento 
		float velocidadAla2 = 0.095f;
		if (wingUp2)
			wingAngle2 += velocidadAla2;
		else
			wingAngle2 -= velocidadAla2;

		// Apertura de las alas
		if (wingAngle2 > 1.5f) wingUp2 = false;
		if (wingAngle2 < -1.5f) wingUp2 = true;



		// Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Piso2.Draw(lightingShader);

		// Galeria
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotación en y
		modelTemp = model = glm::rotate(model, glm::radians(rotDogX), glm::vec3(1.0f, 0.0f, 0.0f)); //Rotación en x 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Galeria.Draw(lightingShader);
		// Piso
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);
		


		//Lámpara 1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, -1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Lampara.Draw(shader);

		//Lámpara 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.0f, -10.0f));
		model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Lampara.Draw(shader);

		// --- TORSO DEL PERRO ---
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.5f, 60.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		PerroTor.Draw(shader);

		// --- CABEZA DEL PERRO ---
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.5f, 60.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		PerroCab.Draw(shader);

		// --- COLA DEL PERRO ---
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.5f, 60.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		PerroCol.Draw(shader);

		// --- PATA DELANTERA DERECHA DEL PERRO ---
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.5f, 60.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		PerroPatFD.Draw(shader);

		// --- PATA DELANTERA IZQUIERDA DEL PERRO ---
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.5f, 60.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		PerroPatFI.Draw(shader);

		// --- PATA TRASERA DERECHA DEL PERRO ---
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.5f, 60.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		PerroPatTD.Draw(shader);

		// --- PATA TRASERA IZQUIERDA DEL PERRO ---
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.5f, 60.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		PerroPatTI.Draw(shader);


		//Arbol 1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Arbol.Draw(shader);

		//Arbol 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.5f, 30.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Arbol.Draw(shader);

		//Arbol 3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.5f, -50.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Arbol.Draw(shader);
		//Manos
		model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 8.7f, 25.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Hands.Draw(shader);
		//Demon
		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 8.40f, -6.8f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.30f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Demon.Draw(shader);

		//Witcher
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.30f, -15.0f));
		model = glm::rotate(model, glm::radians(65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Witch.Draw(shader);
		//Ritual
		model = glm::translate(glm::mat4(1.0f), glm::vec3(23.8f, 0.7f, -14.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Ritual.Draw(shader);
		//cadaver
		model = glm::translate(glm::mat4(1.0f), glm::vec3(23.9f, 2.30f, -15.5f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Corpse.Draw(shader);

		// --- MATRIZ BASE DEL PRIMER MURCIELAGO ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(batPosX, batPosY, batPosZ));
		model = glm::rotate(model, glm::radians(batAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));

		// --- TORSO ---
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		BatCuerpo.Draw(shader);

		// --- COLA ---
		glm::mat4 tail = model;
		tail = glm::translate(tail, glm::vec3(0.0f, -0.05f, -0.20f)); // más unida al cuerpo
		tail = glm::rotate(tail, glm::radians(tailSwing), glm::vec3(0.0f, 1.0f, 0.0f)); // leve balanceo lateral
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(tail));
		BatAguja.Draw(shader);

		// --- Ala derecha ---
		glm::mat4 rightWing = glm::translate(model, glm::vec3(0.35f, 0.05f, 0.0f));
		// primero abre/cierra
		rightWing = glm::rotate(rightWing, glm::radians(-wingAngle * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// luego sube/baja un poco
		rightWing = glm::rotate(rightWing, glm::radians(-wingAngle * 0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(rightWing));
		BatAlader.Draw(shader);

		// --- Ala izquierda ---
		glm::mat4 leftWing = glm::translate(model, glm::vec3(-0.35f, 0.05f, 0.0f));
		// abre/cierra
		leftWing = glm::rotate(leftWing, glm::radians(wingAngle * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// sube/baja
		leftWing = glm::rotate(leftWing, glm::radians(wingAngle * 0.4f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(leftWing));
		BatAlaizq.Draw(shader);

		// --- MATRIZ BASE DEL SEGUNDO MURCIELAGO ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(bat2PosX, bat2PosY, bat2PosZ));
		model = glm::rotate(model, glm::radians(bat2Angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));

		// --- TORSO ---
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		BatCuerpo.Draw(shader);

		// --- COLA ---
		glm::mat4 tail2 = model;
		tail2 = glm::translate(tail2, glm::vec3(0.0f, -0.05f, -0.20f)); // más unida al cuerpo
		tail2 = glm::rotate(tail2, glm::radians(tailSwing), glm::vec3(0.0f, 1.0f, 0.0f)); // leve balanceo lateral
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(tail2));
		BatAguja.Draw(shader);

		// --- Ala derecha ---
		glm::mat4 rightWing2 = glm::translate(model, glm::vec3(0.35f, 0.05f, 0.0f));
		// primero abre/cierra
		rightWing2 = glm::rotate(rightWing2, glm::radians(-wingAngle * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// luego sube/baja un poco
		rightWing2 = glm::rotate(rightWing2, glm::radians(-wingAngle * 0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(rightWing2));
		BatAlader.Draw(shader);

		// --- Ala izquierda ---
		glm::mat4 leftWing2 = glm::translate(model, glm::vec3(-0.35f, 0.05f, 0.0f));
		// abre/cierra
		leftWing2 = glm::rotate(leftWing2, glm::radians(wingAngle * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// sube/baja
		leftWing2 = glm::rotate(leftWing2, glm::radians(wingAngle * 0.4f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(leftWing2));
		BatAlaizq.Draw(shader);



		// Sofa 1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.5f, 52.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Sofa.Draw(shader);


		// Sofa 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.5f, 45.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Sofa.Draw(shader);

		// Sofa 3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.5f, 40.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Sofa.Draw(shader);

		// Sofa 4
		model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.5f, 35.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Sofa.Draw(shader);

		// Recepcion
		model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.5f, 51.5f));
		model = glm::rotate(model, glm::radians(350.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Recepcion.Draw(shader);

		// Mesa Picnic
		model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 0.5f, 45.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Mesa.Draw(shader);

		// ===== PERSONA (torso como base/padre) =====
		glm::mat4 personaBase = glm::mat4(1.0f);
		personaBase = glm::translate(personaBase, glm::vec3(perPosX, perPosY, perPosZ));
		personaBase = glm::rotate(personaBase, glm::radians(perRotY), glm::vec3(0, 1, 0));
		personaBase = glm::rotate(personaBase, glm::radians(perRotX), glm::vec3(1, 0, 0));
		personaBase = glm::scale(personaBase, glm::vec3(0.5f));   // ajusta a tu escala

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(personaBase));
		Ptorso.Draw(lightingShader);

		// Cabeza
		{
			glm::mat4 m = personaBase;
			// m = glm::translate(m, glm::vec3(0.0f, /*offsetY*/, /*offsetZ*/));
			m = glm::rotate(m, glm::radians(perHead), glm::vec3(1, 0, 0));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
			Pcabeza.Draw(lightingShader);
		}

		// Hombro izquierdo
		{
			glm::mat4 m = personaBase;
			// m = glm::translate(m, glm::vec3(/*offset hombro izq*/));
			m = glm::rotate(m, glm::radians(perArmL), glm::vec3(1, 0, 0));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
			Phombroizq.Draw(lightingShader);
		}

		// Hombro derecho
		{
			glm::mat4 m = personaBase;
			// m = glm::translate(m, glm::vec3(/*offset hombro der*/));
			m = glm::rotate(m, glm::radians(perArmR), glm::vec3(1, 0, 0));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
			Phombroder.Draw(lightingShader);
		}

		// Antebrazo izq (fijo por ahora)
		{
			glm::mat4 m = personaBase;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
			Pantebrasizq.Draw(lightingShader);
		}

		// Antebrazo der (fijo por ahora)
		{
			glm::mat4 m = personaBase;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
			Pantebrasder.Draw(lightingShader);
		}

		// Muslo izquierdo
		{
			glm::mat4 m = personaBase;
			// m = glm::translate(m, glm::vec3(/*offset cadera izq*/));
			m = glm::rotate(m, glm::radians(perLegL), glm::vec3(1, 0, 0));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
			Pmusloizq.Draw(lightingShader);
		}

		// Muslo derecho
		{
			glm::mat4 m = personaBase;
			// m = glm::translate(m, glm::vec3(/*offset cadera der*/));
			m = glm::rotate(m, glm::radians(perLegR), glm::vec3(1, 0, 0));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
			Pmusloder.Draw(lightingShader);
		}

		// Rodillas (fijas por ahora)
		{
			glm::mat4 m = personaBase;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
			Prodillaizq.Draw(lightingShader);
		}
		{
			glm::mat4 m = personaBase;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
			Prodillader.Draw(lightingShader);
		}



		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0); //activa o desactiva la transparencia, si se desactiva el interior de la galeria se vera negro
		//Ball.Draw(lightingShader);

		// Restaura configuración normal
		glDisable(GL_BLEND);

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// Draw skybox as last
		glDepthFunc(GL_LEQUAL); //hace que no interfiera con otros objetos //Función de profundidad
		skyboxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyBoxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		// Swap the buffers
		glDeleteVertexArrays(1, &VAO);
		glfwSwapBuffers(window);

	}
	/////////////////Borramos buffers/////////////////
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyBoxVAO);
	glDeleteBuffers(1, &skyBoxVAO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	////Dog Controls
	//if (keys[GLFW_KEY_1])
	//{
	//	angle += speed;  // Incrementa el ángulo para el movimiento circular
	//	//dogPosX = radio * cos(angle); // Posición en X
	//	//dogPosZ = radio * sin(angle); // Posición en Z
	//	rotDog += 0.05f;
	//	FLegs = 15.0f * sin(rotDog * 0.05f); // Mueve las patas delanteras
	//	RLegs = 15.0f * sin(rotDog * 0.05f);  // Mueve las patas traseras
	//	// Mantener circularAngle en el rango [0, 2π]
	//	if (angle > 2 * 3.15) {
	//		angle -= 2 * 3.15;
	//	}
	//}

	//if (keys[GLFW_KEY_2]) rotDogX += 0.1f;
	//if (keys[GLFW_KEY_3]) rotDogX -= 0.1f;

	//if (keys[GLFW_KEY_4]) head += 0.1f;
	//if (keys[GLFW_KEY_5]) head -= 0.1f;

	//if (keys[GLFW_KEY_6]) tail += 0.1f;
	//if (keys[GLFW_KEY_7]) tail -= 0.1f;

	//if (keys[GLFW_KEY_8]) RLegs += 0.1f;
	//if (keys[GLFW_KEY_9]) RLegs -= 0.1f;

	//if (keys[GLFW_KEY_Z]) FLegsL += 0.1f;
	//if (keys[GLFW_KEY_X]) FLegsL -= 0.1f;

	//if (keys[GLFW_KEY_M]) FLegsR += 0.1f;
	//if (keys[GLFW_KEY_N]) FLegsR -= 0.1f;

	//if (keys[GLFW_KEY_H]) dogPosZ += 0.001;
	//if (keys[GLFW_KEY_Y]) dogPosZ -= 0.001;

	//if (keys[GLFW_KEY_G]) dogPosX -= 0.001;
	//if (keys[GLFW_KEY_J]) dogPosX += 0.001;

	//if (keys[GLFW_KEY_C]) dogPosY -= 0.001;
	//if (keys[GLFW_KEY_V]) dogPosY += 0.001;

	// ===== Controles PERSONA =====
	if (keys[GLFW_KEY_1]) { // caminar simple con balanceo
		perRotY += 0.05f;
		float osc = 15.0f * sin(perRotY * 0.05f);
		perArmL = osc;
		perArmR = -osc;
		perLegL = -osc;
		perLegR = osc;
	}

	if (keys[GLFW_KEY_2]) perRotX += 0.1f;
	if (keys[GLFW_KEY_3]) perRotX -= 0.1f;

	if (keys[GLFW_KEY_4]) perHead += 0.1f;
	if (keys[GLFW_KEY_5]) perHead -= 0.1f;

	if (keys[GLFW_KEY_Z]) perArmL += 0.1f;
	if (keys[GLFW_KEY_X]) perArmL -= 0.1f;

	if (keys[GLFW_KEY_M]) perArmR += 0.1f;
	if (keys[GLFW_KEY_N]) perArmR -= 0.1f;

	// piernas (ajústales teclas si quieres independientes)
	if (keys[GLFW_KEY_8]) perLegL += 0.1f;
	if (keys[GLFW_KEY_9]) perLegL -= 0.1f;

	if (keys[GLFW_KEY_H]) perPosZ += 0.001f;
	if (keys[GLFW_KEY_Y]) perPosZ -= 0.001f;
	if (keys[GLFW_KEY_G]) perPosX -= 0.001f;
	if (keys[GLFW_KEY_J]) perPosX += 0.001f;
	if (keys[GLFW_KEY_C]) perPosY -= 0.001f;
	if (keys[GLFW_KEY_V]) perPosY += 0.001f;


	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (key == GLFW_KEY_L && GLFW_PRESS == action)
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (key == GLFW_KEY_K && GLFW_PRESS == action)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame(); //Almacena cada frame
		}

	}

	if (key == GLFW_KEY_Q && GLFW_PRESS == action) {
		SaveAnimation();  // Guarda la animación en "Animacion.txt"
	}

	if (key == GLFW_KEY_R && GLFW_PRESS == action) {

		resetElements();  // Resetear los elementos a los primeros keyframes cargados
		LoadAnimation(); //Carga la animación por medio del archivo previamente guardado
		PrintAnimation(); //Imprime en terminar los valores del archivo
	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (key == GLFW_KEY_SPACE && GLFW_PRESS == action)
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}


}
void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			////Draw animation
			//dogPosX += KeyFrame[playIndex].incX;
			//dogPosY += KeyFrame[playIndex].incY;
			//dogPosZ += KeyFrame[playIndex].incZ;
			//head += KeyFrame[playIndex].headInc;
			//tail += KeyFrame[playIndex].tailInc;
			//FLegsL += KeyFrame[playIndex].FLegsLInc;
			//FLegsR += KeyFrame[playIndex].FLegsRInc;
			//RLegs += KeyFrame[playIndex].RLegsInc;
			//FLegs += KeyFrame[playIndex].FLegsInc;
			//rotDog += KeyFrame[playIndex].rotDogInc;
			//rotDogX += KeyFrame[playIndex].rotDogXInc;

			//i_curr_steps++;
			perPosX += KeyFrame[playIndex].incX;
			perPosY += KeyFrame[playIndex].incY;
			perPosZ += KeyFrame[playIndex].incZ;

			perHead += KeyFrame[playIndex].headInc;
			perArmL += KeyFrame[playIndex].FLegsLInc;
			perArmR += KeyFrame[playIndex].FLegsRInc;
			// Si no usas piernas en KF, ignora RLegsInc/FLegsInc

			perRotY += KeyFrame[playIndex].rotDogInc;
			perRotX += KeyFrame[playIndex].rotDogXInc;
			i_curr_steps++;
		}

	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}