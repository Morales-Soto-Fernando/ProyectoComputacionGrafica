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
 // --- Audio ---
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
// ---

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
void AnimatePerson();
void AnimateSkeleton();
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(27.0f, 4.0f, 68.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// --- Variables Globales de Audio ---
ma_engine audio_engine; // El motor principal de audio
ma_sound bgm_sound;     // Sonido específico para música de fondo (para poder hacer loop)
ma_sound steps_sound;
ma_sound ladrido_sound;
// 
// 
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

// --- ¡CAMBIO 1: 8 ESTADOS PARA RUTA COMPLETA! ---
enum PersonState {
	WALK_Z_NEG,  // Tramo 1: Camina en -Z
	TURN_TO_X,   // Tramo 2: Gira 90 izq
	WALK_X_NEG,  // Tramo 3: Camina en -X
	TURN_180_X,  // Tramo 4: Gira 180 (para regresar por X)
	WALK_X_POS,  // Tramo 5: Camina en +X (regreso)
	TURN_TO_Z,   // Tramo 6: Gira 90 der (para regresar por Z)
	WALK_Z_POS,  // Tramo 7: Camina en +Z (regreso)
	TURN_180_Z   // Tramo 8: Gira 180 (para reiniciar ciclo)
};
PersonState personState;

float personWalkSpeed = 2.0f;     // Unidades por segundo (un poco más lento)
float personTurnSpeed = 90.0f;    // 90 grados por segundo
float personCycleSpeed = 8.0f;    // Velocidad del ciclo de caminata
float personWalkAngle = 0.0f;     // El "reloj" para la oscilación
float stepRate = 2.2f;
// --- Variables de Articulaciones ---
float perArmL = 0.0f, perArmR = 0.0f;     // Hombros
float perLegL = 0.0f, perLegR = 0.0f;     // Caderas
float perKneeL = 0.0f, perKneeR = 0.0f;    // Rodillas
float perElbowL = 0.0f, perElbowR = 0.0f; // Codos

// --- ¡CAMBIO 2: NUEVAS VARIABLES DE DESTINO! ---
float personTargetZ_1;   // 60 -> 20
float personTargetRot_1; // 180 -> 270
float personTargetX_1;   // 20 -> -20
float personTargetRot_2; // 270 -> 90
float personTargetX_2;   // -20 -> 20
float personTargetRot_3; // 90 -> 0 
float personTargetZ_2;   // 20 -> 60
float personTargetRot_4; // 0 -> 180
// ===============================================
// ===== VARIABLES DEL ESQUELETO (BAILE) =====
// ===============================================
// --- Posición base (tomada de tu código estático)
float skelPosX = 12.0f;
float skelPosY = 0.5f;
float skelPosZ = 45.0f;
float skelBaseRotY = 45.0f; // Rotación estática
// --- Variables de animación
float danceAngle = 0.0f;    // El "reloj" para el baile
float danceRotY = 0.0f;     // Giro del cuerpo
float skelArmL = 0.0f, skelArmR = 0.0f;
float skelLegL = 0.0f, skelLegR = 0.0f;
//KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 60
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

	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;

	head = KeyFrame[0].head;
	tail = KeyFrame[0].tail;
	FLegs = KeyFrame[0].FLegs;
	FLegsL = KeyFrame[0].FLegsL;
	FLegsR = KeyFrame[0].FLegsR;
	RLegs = KeyFrame[0].RLegs;

	rotDog = KeyFrame[0].rotDog;
	rotDogX = KeyFrame[0].rotDogX;

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
void setupDogAnimation() {
	// Reiniciamos el índice
	FrameIndex = 0;
	float stepSize = 5.0f;

	// Variables para rastrear la posición y rotación actual
	float currentX = 30.0f;
	float currentY = 0.5f;
	float currentZ = 60.0f;
	float currentRotY = 0.0f;

	// --- KEYFRAME 0: Posición Inicial (Quieto) ---
	KeyFrame[FrameIndex].dogPosX = currentX;
	KeyFrame[FrameIndex].dogPosY = currentY;
	KeyFrame[FrameIndex].dogPosZ = currentZ;
	KeyFrame[FrameIndex].rotDog = currentRotY;
	KeyFrame[FrameIndex].rotDogX = 0.0f;
	KeyFrame[FrameIndex].FLegsL = 0.0f; KeyFrame[FrameIndex].FLegsR = 0.0f;
	KeyFrame[FrameIndex].RLegs = 0.0f;
	KeyFrame[FrameIndex].head = 0.0f; KeyFrame[FrameIndex].tail = 0.0f;
	FrameIndex++;

	// --- 1. CAMINAR 5 PASOS (Z+) ---
	for (int i = 1; i <= 5; i++) {
		currentZ += stepSize; // Avanza en Z+
		KeyFrame[FrameIndex].dogPosX = currentX;
		KeyFrame[FrameIndex].dogPosY = currentY;
		KeyFrame[FrameIndex].dogPosZ = currentZ;
		KeyFrame[FrameIndex].rotDog = currentRotY;
		KeyFrame[FrameIndex].rotDogX = 0.0f;

		if (i % 2 != 0) { // Paso impar
			KeyFrame[FrameIndex].FLegsL = 10.0f; KeyFrame[FrameIndex].FLegsR = -10.0f;
			KeyFrame[FrameIndex].RLegs = -10.0f;
			KeyFrame[FrameIndex].head = 8.0f; KeyFrame[FrameIndex].tail = 15.0f;
		}
		else { // Paso par
			KeyFrame[FrameIndex].FLegsL = -10.0f; KeyFrame[FrameIndex].FLegsR = 10.0f;
			KeyFrame[FrameIndex].RLegs = 10.0f;
			KeyFrame[FrameIndex].head = -8.0f; KeyFrame[FrameIndex].tail = -15.0f;
		}
		FrameIndex++;
	}

	// --- 2. GIRO DERECHA (-90°) ---
	currentRotY = -90.0f;
	KeyFrame[FrameIndex] = KeyFrame[FrameIndex - 1]; // Copia pos anterior
	KeyFrame[FrameIndex].rotDog = currentRotY;
	KeyFrame[FrameIndex].FLegsL = 0.0f; KeyFrame[FrameIndex].FLegsR = 0.0f; // Patas quietas
	KeyFrame[FrameIndex].RLegs = 0.0f;
	KeyFrame[FrameIndex].head = 0.0f; KeyFrame[FrameIndex].tail = 0.0f;
	FrameIndex++;

	// --- 3. CAMINAR 5 PASOS (X-) ---
	for (int i = 1; i <= 5; i++) {
		currentX -= stepSize; // Avanza en X-
		KeyFrame[FrameIndex].dogPosX = currentX;
		KeyFrame[FrameIndex].dogPosY = currentY;
		KeyFrame[FrameIndex].dogPosZ = currentZ;
		KeyFrame[FrameIndex].rotDog = currentRotY;
		KeyFrame[FrameIndex].rotDogX = 0.0f;

		if (i % 2 != 0) {
			KeyFrame[FrameIndex].FLegsL = 10.0f; KeyFrame[FrameIndex].FLegsR = -10.0f;
			KeyFrame[FrameIndex].RLegs = -10.0f;
			KeyFrame[FrameIndex].head = 8.0f; KeyFrame[FrameIndex].tail = 15.0f;
		}
		else {
			KeyFrame[FrameIndex].FLegsL = -10.0f; KeyFrame[FrameIndex].FLegsR = 10.0f;
			KeyFrame[FrameIndex].RLegs = 10.0f;
			KeyFrame[FrameIndex].head = -8.0f; KeyFrame[FrameIndex].tail = -15.0f;
		}
		FrameIndex++;
	}

	// --- 3. (cont.) SENTARSE ---
	KeyFrame[FrameIndex] = KeyFrame[FrameIndex - 1];
	KeyFrame[FrameIndex].FLegsL = 0.0f; KeyFrame[FrameIndex].FLegsR = 0.0f;
	KeyFrame[FrameIndex].head = 0.0f; KeyFrame[FrameIndex].tail = 0.0f;
	KeyFrame[FrameIndex].rotDogX = -35.0f;
	KeyFrame[FrameIndex].dogPosY = 0.0f;
	KeyFrame[FrameIndex].RLegs = 45.0f;
	FrameIndex++;


	// ===================================================
	// --- INICIO DE LA NUEVA SECUENCIA ---
	// ===================================================

	// --- 4. LEVANTARSE ---
	KeyFrame[FrameIndex] = KeyFrame[FrameIndex - 1]; // Copia pos sentada
	KeyFrame[FrameIndex].rotDogX = 0.0f;  // Enderezar espalda
	KeyFrame[FrameIndex].dogPosY = 0.5f;  // Subir el cuerpo
	KeyFrame[FrameIndex].RLegs = 0.0f;    // Patas en 0
	FrameIndex++;

	// --- 5. DAR VUELTA 180° ---
	currentRotY = 90.0f; // -90 + 180 = 90
	KeyFrame[FrameIndex] = KeyFrame[FrameIndex - 1];
	KeyFrame[FrameIndex].rotDog = currentRotY;
	FrameIndex++;

	// --- 6. CAMINAR 10 PASOS (X+) ---
	for (int i = 1; i <= 10; i++) {
		currentX += stepSize; // Avanza en X+ (opuesto a X-)
		KeyFrame[FrameIndex].dogPosX = currentX;
		KeyFrame[FrameIndex].dogPosY = currentY;
		KeyFrame[FrameIndex].dogPosZ = currentZ;
		KeyFrame[FrameIndex].rotDog = currentRotY;
		KeyFrame[FrameIndex].rotDogX = 0.0f;

		if (i % 2 != 0) {
			KeyFrame[FrameIndex].FLegsL = 10.0f; KeyFrame[FrameIndex].FLegsR = -10.0f;
			KeyFrame[FrameIndex].RLegs = -10.0f;
			KeyFrame[FrameIndex].head = 8.0f; KeyFrame[FrameIndex].tail = 15.0f;
		}
		else {
			KeyFrame[FrameIndex].FLegsL = -10.0f; KeyFrame[FrameIndex].FLegsR = 10.0f;
			KeyFrame[FrameIndex].RLegs = 10.0f;
			KeyFrame[FrameIndex].head = -8.0f; KeyFrame[FrameIndex].tail = -15.0f;
		}
		FrameIndex++;
	}

	// --- 7. GIRO IZQUIERDA (a 180°) ---
	currentRotY = 180.0f; // 90 + 90 = 180
	KeyFrame[FrameIndex] = KeyFrame[FrameIndex - 1];
	KeyFrame[FrameIndex].rotDog = currentRotY;
	KeyFrame[FrameIndex].FLegsL = 0.0f; KeyFrame[FrameIndex].FLegsR = 0.0f;
	KeyFrame[FrameIndex].RLegs = 0.0f;
	KeyFrame[FrameIndex].head = 0.0f; KeyFrame[FrameIndex].tail = 0.0f;
	FrameIndex++;

	// --- 8. CAMINAR 5 PASOS (Z-) ---
	for (int i = 1; i <= 5; i++) {
		currentZ -= stepSize; // Avanza en Z- (opuesto a Z+)
		KeyFrame[FrameIndex].dogPosX = currentX;
		KeyFrame[FrameIndex].dogPosY = currentY;
		KeyFrame[FrameIndex].dogPosZ = currentZ;
		KeyFrame[FrameIndex].rotDog = currentRotY;
		KeyFrame[FrameIndex].rotDogX = 0.0f;

		if (i % 2 != 0) {
			KeyFrame[FrameIndex].FLegsL = 10.0f; KeyFrame[FrameIndex].FLegsR = -10.0f;
			KeyFrame[FrameIndex].RLegs = -10.0f;
			KeyFrame[FrameIndex].head = 8.0f; KeyFrame[FrameIndex].tail = 15.0f;
		}
		else {
			KeyFrame[FrameIndex].FLegsL = -10.0f; KeyFrame[FrameIndex].FLegsR = 10.0f;
			KeyFrame[FrameIndex].RLegs = 10.0f;
			KeyFrame[FrameIndex].head = -8.0f; KeyFrame[FrameIndex].tail = -15.0f;
		}
		FrameIndex++;
	}

	// --- 9. GIRO IZQUIERDA (a -90°) ---
	currentRotY = 270.0f; // 180 + 90 = 270, que es lo mismo que -90
	KeyFrame[FrameIndex] = KeyFrame[FrameIndex - 1];
	KeyFrame[FrameIndex].rotDog = currentRotY;
	KeyFrame[FrameIndex].FLegsL = 0.0f; KeyFrame[FrameIndex].FLegsR = 0.0f;
	KeyFrame[FrameIndex].RLegs = 0.0f;
	KeyFrame[FrameIndex].head = 0.0f; KeyFrame[FrameIndex].tail = 0.0f;
	FrameIndex++;

	// --- 10. CAMINAR 5 PASOS (X-) ---
	// En este punto, currentZ = 60
	for (int i = 1; i <= 5; i++) {
		currentX -= stepSize; // Avanza en X-
		KeyFrame[FrameIndex].dogPosX = currentX;
		KeyFrame[FrameIndex].dogPosY = currentY;
		KeyFrame[FrameIndex].dogPosZ = currentZ;
		KeyFrame[FrameIndex].rotDog = currentRotY;
		KeyFrame[FrameIndex].rotDogX = 0.0f;

		if (i % 2 != 0) {
			KeyFrame[FrameIndex].FLegsL = 10.0f; KeyFrame[FrameIndex].FLegsR = -10.0f;
			KeyFrame[FrameIndex].RLegs = -10.0f;
			KeyFrame[FrameIndex].head = 8.0f; KeyFrame[FrameIndex].tail = 15.0f;
		}
		else {
			KeyFrame[FrameIndex].FLegsL = -10.0f; KeyFrame[FrameIndex].FLegsR = 10.0f;
			KeyFrame[FrameIndex].RLegs = 10.0f;
			KeyFrame[FrameIndex].head = -8.0f; KeyFrame[FrameIndex].tail = -15.0f;
		}
		FrameIndex++;
	}
	// Al final de este loop, currentX debería ser 30.0 (¡Posición inicial de X!)

	// --- 11. GIRO IZQUIERDA (a 0°) ---
	currentRotY = 360.0f; // -90 + 90 = 0
	KeyFrame[FrameIndex] = KeyFrame[FrameIndex - 1];
	KeyFrame[FrameIndex].rotDog = currentRotY;
	KeyFrame[FrameIndex].FLegsL = 0.0f; KeyFrame[FrameIndex].FLegsR = 0.0f;
	KeyFrame[FrameIndex].RLegs = 0.0f;
	KeyFrame[FrameIndex].head = 0.0f; KeyFrame[FrameIndex].tail = 0.0f;
	FrameIndex++;
	// En este punto, estamos en (30, 0.5, 60) con RotY 0.0 ¡Posición inicial!

	// --- 12. SENTARSE (FINAL) ---
	KeyFrame[FrameIndex] = KeyFrame[FrameIndex - 1];
	KeyFrame[FrameIndex].FLegsL = 0.0f; KeyFrame[FrameIndex].FLegsR = 0.0f;
	KeyFrame[FrameIndex].head = 0.0f; KeyFrame[FrameIndex].tail = 0.0f;
	KeyFrame[FrameIndex].rotDogX = -35.0f;
	KeyFrame[FrameIndex].dogPosY = 0.0f;
	KeyFrame[FrameIndex].RLegs = 45.0f;
	FrameIndex++;

	printf("Animacion extendida cargada. Total de frames: %d\n", FrameIndex);
}
#include "stb_image.h"

static GLuint LoadCubemapSTB(const std::vector<const char*>& faces) {
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	int w, h, comp;
	stbi_set_flip_vertically_on_load(false);  // ¡no voltear cubemaps!

	for (GLuint i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i], &w, &h, &comp, 0);
		if (!data) {
			std::cerr << "CUBEMAP: fallo al cargar cara: " << faces[i] << std::endl;
			glDeleteTextures(1, &texID);
			return 0;
		}
		GLenum fmt = (comp == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return texID;
}

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
	if (ma_engine_init(NULL, &audio_engine) != MA_SUCCESS) {
		std::cout << "Error al inicializar el motor de audio." << std::endl;
		return -1;
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

	// Escultura
	Model Viecab((char*)"Models/Galeria/Escultura/viejocabeza.obj");
	Model Vietor((char*)"Models/Galeria/Escultura/viejotorso.obj");
	Model Vieanbder((char*)"Models/Galeria/Escultura/viejoantebrader.obj");
	Model Vieanbizq((char*)"Models/Galeria/Escultura/viejoantebrazoizq.obj");
	Model Viebrader((char*)"Models/Galeria/Escultura/viejobrazoder.obj");
	Model Viebraizq((char*)"Models/Galeria/Escultura/viejobrazoizq.obj");
	Model Viemusder((char*)"Models/Galeria/Escultura/viejomusloder.obj");
	Model Viemusizq((char*)"Models/Galeria/Escultura/viejomusloizq.obj");
	Model Viepieder((char*)"Models/Galeria/Escultura/viejopiernader.obj");
	Model Viepieizq((char*)"Models/Galeria/Escultura/viejopiernaizq.obj");

	// Esqueleto
	Model Etorso((char*)"Models/Galeria/Esqueleto/Etorso.obj");
	Model Ecabeza((char*)"Models/Galeria/Esqueleto/Ecabeza.obj");
	Model Ebrader((char*)"Models/Galeria/Esqueleto/Ebrasoder.obj");
	Model Ebraizq((char*)"Models/Galeria/Esqueleto/Ebrasoizq.obj");
	Model Epelvis((char*)"Models/Galeria/Esqueleto/Epelvis.obj");
	Model Epieder((char*)"Models/Galeria/Esqueleto/Epiernader.obj");
	Model Epieizq((char*)"Models/Galeria/Esqueleto/Epiernaizq.obj");



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
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
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
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	///////////////////////////SKYBOX/////////////////////////
	GLuint skyBoxVBO, skyBoxVAO;
	glGenVertexArrays(1, &skyBoxVAO);
	glGenBuffers(1, &skyBoxVBO);
	glBindVertexArray(skyBoxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Load textures
	// CÓDIGO CORREGIDO (Orden Correcto)
	vector < const GLchar*> faces;
	faces.push_back("SkyBox/px.jpg"); // Right (+X)
	faces.push_back("SkyBox/nx.jpg"); // Left (-X)
	faces.push_back("SkyBox/py.jpg"); // Top (+Y)
	faces.push_back("SkyBox/ny.jpg"); // Bottom (-Y)
	faces.push_back("SkyBox/pz.jpg"); // Front (+Z)
	faces.push_back("SkyBox/nz.jpg"); // Back (-Z)

	GLuint cubemapTexture = LoadCubemapSTB(faces);
	if (cubemapTexture == 0) {
		std::cerr << "ERROR: el cubemap no se cargo (id=0). Revisa rutas/nombres.\n";
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	/////////////////////////////////////////////////////////
	ma_result result = ma_sound_init_from_file(&audio_engine, "Audio/Fondo.mp3", 0, NULL, NULL, &bgm_sound);
	if (result == MA_SUCCESS) {
		ma_sound_set_looping(&bgm_sound, MA_TRUE); // Activar loop
		ma_sound_set_volume(&bgm_sound, 0.5f);     // Ajustar volumen (0.0 a 1.0)
		ma_sound_start(&bgm_sound);                // Iniciar reproducción
	}
	result = ma_sound_init_from_file(&audio_engine, "Audio/pasos.mp3", 0, NULL, NULL, &steps_sound);
	if (result == MA_SUCCESS) {
		ma_sound_set_looping(&steps_sound, MA_TRUE); // Importante: que se repita mientras caminas
		ma_sound_set_volume(&steps_sound, 1.0f);     // Ajusta el volumen si es necesario
		// NO lo iniciamos aquí con ma_sound_start, esperaremos a movernos
	}
	result = ma_sound_init_from_file(&audio_engine, "Audio/Gruñidos.mp3", 0, NULL, NULL, &ladrido_sound);
	if (result == MA_SUCCESS) {
		ma_sound_set_looping(&ladrido_sound, MA_TRUE);
		ma_sound_set_volume(&ladrido_sound, 0.8f); // Ajusta el volumen (0.0 a 1.0)
	}
	else {
		std::cout << "Error al cargar Gruñidos.mp3" << std::endl;
	}
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	setupDogAnimation();
	// Establecemos la posición inicial del perro igual al primer keyframe
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	rotDog = KeyFrame[0].rotDog;
	rotDogX = KeyFrame[0].rotDogX;
	FLegsL = KeyFrame[0].FLegsL;
	FLegsR = KeyFrame[0].FLegsR;
	RLegs = KeyFrame[0].RLegs;
	head = KeyFrame[0].head; // <-- AÑADIDO
	tail = KeyFrame[0].tail;
	// --------------------------------
	// --- Establecemos la posición y estado inicial de la PERSONA ---
	perPosX = 20.0f; // <-- Más a la izquierda (antes era 25.0)
	perPosY = 0.3f;
	perPosZ = 60.0f;
	perRotY = 180.0f; // Mirando hacia Z negativo (Estado inicial)

	// --- Definimos los objetivos de la FSM ---
	// (Inicia en Z=60, X=20)
	personTargetZ_1 = -13.0f;  // TRAMO 1: (Z) 60 a -15 
	personTargetRot_1 = 270.0f;  // TRAMO 2: (Rot) 180 a 270 (Gira 90 izq)
	personTargetX_1 = -22.0f;  // TRAMO 3: (X) 20 a -23 
	personTargetRot_2 = 90.0f;   // TRAMO 4: (Rot) 270 a 90 (Giro 180)
	personTargetX_2 = 20.0f;   // TRAMO 5: (X) -23 a 20 (Regreso)
	personTargetRot_3 = 0.0f;    // TRAMO 6: (Rot) 90 a 0 (Giro 90 der)
	personTargetZ_2 = 60.0f;   // TRAMO 7: (Z) -15 a 60 (Regreso)
	personTargetRot_4 = 180.0f;  // TRAMO 8: (Rot) 0 a 180 (Giro 180)

	// --- ESTADO INICIAL ---
	personState = WALK_Z_NEG; // Inicia la animación automáticamente
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
		AnimatePerson();
		AnimateSkeleton();
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// ###########################################################
		// ### INICIO DE TODO EL DIBUJADO ###
		// Se activa UN SOLO SHADER para TODOS los modelos
		// ###########################################################
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light (OSCURA)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.15f, 0.15f, 0.15f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
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


		// SpotLight (LINTERNA INTENSA)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 2.50f, 2.50f, 2.50f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 2.50f, 2.50f, 2.50f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.9f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(14.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations (SOLO UNA VEZ)
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
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Piso2.Draw(lightingShader);

		// Galeria
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// --- LUZ ESPECIAL PARA GALERIA ---
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.4f, 0.4f, 0.4f);
		Galeria.Draw(lightingShader);
		// --- LUZ OSCURA PARA TODO LO DEMÁS ---
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.15f, 0.15f, 0.15f);

		// Piso
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		// ###########################################################
		// ### YA NO SE CAMBIA DE SHADER AQUÍ ###
		// ###########################################################

		//Lámpara 1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, -1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Lampara.Draw(lightingShader);

		//Lámpara 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.0f, -10.0f));
		model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Lampara.Draw(lightingShader);

		// --- DIBUJADO DEL PERRO ANIMADO ---
		glm::mat4 modelDog = glm::mat4(1.0f);
		// 1. Transformaciones Base del Perro (afectan a todo el cuerpo)
		modelDog = glm::translate(modelDog, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelDog = glm::rotate(modelDog, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		modelDog = glm::rotate(modelDog, glm::radians(rotDogX), glm::vec3(1.0f, 0.0f, 0.0f));
		modelDog = glm::scale(modelDog, glm::vec3(0.9f));

		// 2. Dibujar TORSO (es la base)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
		PerroTor.Draw(lightingShader);

		// 3. Dibujar CABEZA (hijo del torso)
		glm::mat4 modelCabeza = modelDog;
		modelCabeza = glm::rotate(modelCabeza, glm::radians(head), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCabeza));
		PerroCab.Draw(lightingShader);

		// 4. Dibujar COLA
		glm::mat4 modelCola = modelDog;
		modelCola = glm::rotate(modelCola, glm::radians(tail), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCola));
		PerroCol.Draw(lightingShader);

		// --- CORRECCIÓN DE PIVOTE PARA PATAS ---
		glm::vec3 hombroIzquierdo = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 hombroDerecho = glm::vec3(-0.0f, 0.0f, 0.0f);
		glm::vec3 caderaIzquierda = glm::vec3(0.0f, 0.0f, -0.0f);
		glm::vec3 caderaDerecha = glm::vec3(-0.0f, 0.0f, -0.0f);


		// 5. PATA DELANTERA IZQUIERDA (FI)
		glm::mat4 modelPataFI = modelDog;
		modelPataFI = glm::translate(modelPataFI, hombroIzquierdo);
		modelPataFI = glm::rotate(modelPataFI, glm::radians(FLegsL), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPataFI));
		PerroPatFI.Draw(lightingShader);

		// 6. PATA DELANTERA DERECHA (FD)
		glm::mat4 modelPataFD = modelDog;
		modelPataFD = glm::translate(modelPataFD, hombroDerecho);
		modelPataFD = glm::rotate(modelPataFD, glm::radians(FLegsR), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPataFD));
		PerroPatFD.Draw(lightingShader);

		// 7. PATA TRASERA IZQUIERDA (TI)
		glm::mat4 modelPataTI = modelDog;
		modelPataTI = glm::translate(modelPataTI, caderaIzquierda);
		modelPataTI = glm::rotate(modelPataTI, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPataTI));
		PerroPatTI.Draw(lightingShader);

		// 8. PATA TRASERA DERECHA (TD)
		glm::mat4 modelPataTD = modelDog;
		modelPataTD = glm::translate(modelPataTD, caderaDerecha);
		if (rotDogX < -10.0f) {
			modelPataTD = glm::rotate(modelPataTD, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else {
			modelPataTD = glm::rotate(modelPataTD, glm::radians(FLegsL), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPataTD));
		PerroPatTD.Draw(lightingShader);


		//Arbol 1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Arbol.Draw(lightingShader);

		//Arbol 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.5f, 30.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Arbol.Draw(lightingShader);

		//Arbol 3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.5f, -50.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Arbol.Draw(lightingShader);

		//Manos
		model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 8.7f, 25.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Hands.Draw(lightingShader);

		//Demon
		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 8.40f, -6.8f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.30f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Demon.Draw(lightingShader);

		//Witcher
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.30f, -15.0f));
		model = glm::rotate(model, glm::radians(65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Witch.Draw(lightingShader);

		//Ritual
		model = glm::translate(glm::mat4(1.0f), glm::vec3(23.8f, 0.7f, -14.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Ritual.Draw(lightingShader);

		//cadaver
		model = glm::translate(glm::mat4(1.0f), glm::vec3(23.9f, 2.30f, -15.5f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Corpse.Draw(lightingShader);

		// --- MATRIZ BASE DEL PRIMER MURCIELAGO ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(batPosX, batPosY, batPosZ));
		model = glm::rotate(model, glm::radians(batAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));

		// --- TORSO ---
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		BatCuerpo.Draw(lightingShader);

		// --- COLA ---
		glm::mat4 tail = model;
		tail = glm::translate(tail, glm::vec3(0.0f, -0.05f, -0.20f));
		tail = glm::rotate(tail, glm::radians(tailSwing), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tail)); // <-- CORREGIDO
		BatAguja.Draw(lightingShader);

		// --- Ala derecha ---
		glm::mat4 rightWing = glm::translate(model, glm::vec3(0.35f, 0.05f, 0.0f));
		rightWing = glm::rotate(rightWing, glm::radians(-wingAngle * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rightWing = glm::rotate(rightWing, glm::radians(-wingAngle * 0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rightWing)); // <-- CORREGIDO
		BatAlader.Draw(lightingShader);

		// --- Ala izquierda ---
		glm::mat4 leftWing = glm::translate(model, glm::vec3(-0.35f, 0.05f, 0.0f));
		leftWing = glm::rotate(leftWing, glm::radians(wingAngle * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		leftWing = glm::rotate(leftWing, glm::radians(wingAngle * 0.4f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(leftWing)); // <-- CORREGIDO
		BatAlaizq.Draw(lightingShader);

		// --- MATRIZ BASE DEL SEGUNDO MURCIELAGO ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(bat2PosX, bat2PosY, bat2PosZ));
		model = glm::rotate(model, glm::radians(bat2Angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));

		// --- TORSO ---
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		BatCuerpo.Draw(lightingShader);

		// --- COLA ---
		glm::mat4 tail2 = model;
		tail2 = glm::translate(tail2, glm::vec3(0.0f, -0.05f, -0.20f));
		tail2 = glm::rotate(tail2, glm::radians(tailSwing), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tail2)); // <-- CORREGIDO
		BatAguja.Draw(lightingShader);

		// --- Ala derecha ---
		glm::mat4 rightWing2 = glm::translate(model, glm::vec3(0.35f, 0.05f, 0.0f));
		rightWing2 = glm::rotate(rightWing2, glm::radians(-wingAngle * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rightWing2 = glm::rotate(rightWing2, glm::radians(-wingAngle * 0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rightWing2)); // <-- CORREGIDO
		BatAlader.Draw(lightingShader);

		// --- Ala izquierda ---
		glm::mat4 leftWing2 = glm::translate(model, glm::vec3(-0.35f, 0.05f, 0.0f));
		leftWing2 = glm::rotate(leftWing2, glm::radians(wingAngle * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		leftWing2 = glm::rotate(leftWing2, glm::radians(wingAngle * 0.4f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(leftWing2)); // <-- CORREGIDO
		BatAlaizq.Draw(lightingShader);



		// Sofa 1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.5f, 52.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Sofa.Draw(lightingShader);


		// Sofa 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.5f, 45.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Sofa.Draw(lightingShader);

		// Sofa 3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.5f, 40.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Sofa.Draw(lightingShader);

		// Sofa 4
		model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.5f, 35.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Sofa.Draw(lightingShader);

		// Recepcion
		model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.5f, 51.5f));
		model = glm::rotate(model, glm::radians(350.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Recepcion.Draw(lightingShader);

		// Mesa Picnic
		model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 0.5f, 45.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // <-- CORREGIDO
		Mesa.Draw(lightingShader);


		// ###########################################################
		// ### YA NO SE CAMBIA DE SHADER AQUÍ ###
		// ###########################################################


		// ===============================================
		// ===== PERSONA (CORRECCIÓN DE ORDEN DE ESCALA) =====
		// ===============================================

		// 1. Matriz Base (Torso) - SIN ESCALA
		glm::mat4 personaBase = glm::mat4(1.0f);
		personaBase = glm::translate(personaBase, glm::vec3(perPosX, perPosY, perPosZ));
		personaBase = glm::rotate(personaBase, glm::radians(perRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		personaBase = glm::rotate(personaBase, glm::radians(perRotX), glm::vec3(1.0f, 0.0f, 0.0f));

		// 1.b. Dibujar Torso (con su propia escala)
		glm::mat4 modelTorso = personaBase;
		modelTorso = glm::scale(modelTorso, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTorso));
		Ptorso.Draw(lightingShader);

		// 2. Cabeza (hija de personaBase)
		glm::mat4 modelCabezaP = personaBase;
		modelCabezaP = glm::translate(modelCabezaP, glm::vec3(0.0f, 0.0f, 0.0f));
		modelCabezaP = glm::rotate(modelCabezaP, glm::radians(perHead), glm::vec3(1.0f, 0.0f, 0.0f));
		modelCabezaP = glm::scale(modelCabezaP, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCabezaP));
		Pcabeza.Draw(lightingShader);

		// --- JERARQUÍA DEL BRAZO IZQUIERDO ---
		// 3. Hombro Izquierdo (Matriz padre para el codo)
		glm::mat4 modelHomL_sinEscala = personaBase;
		modelHomL_sinEscala = glm::translate(modelHomL_sinEscala, glm::vec3(0.0f, 0.0f, 0.0f));
		modelHomL_sinEscala = glm::rotate(modelHomL_sinEscala, glm::radians(perArmL), glm::vec3(1.0f, 0.0f, 0.0f));

		// 7. Antebrazo Izquierdo (hijo de Hombro Izq)
		glm::mat4 modelAntL = modelHomL_sinEscala;
		modelAntL = glm::translate(modelAntL, glm::vec3(-0.30f, 0.30f, -0.70f));
		modelAntL = glm::rotate(modelAntL, glm::radians(perElbowL), glm::vec3(1.0f, 0.0f, 0.0f));
		modelAntL = glm::scale(modelAntL, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAntL));
		Pantebrasizq.Draw(lightingShader);

		// 3.b. Dibujar Hombro Izquierdo (Ahora dibujamos el padre)
		glm::mat4 modelHomL_conEscala = modelHomL_sinEscala;
		modelHomL_conEscala = glm::scale(modelHomL_conEscala, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHomL_conEscala));
		Phombroizq.Draw(lightingShader);

		// --- JERARQUÍA DEL BRAZO DERECHO ---
		// 4. Hombro Derecho (Matriz padre para el codo)
		glm::mat4 modelHomR_sinEscala = personaBase;
		modelHomR_sinEscala = glm::translate(modelHomR_sinEscala, glm::vec3(-0.0f, 0.0f, 0.0f));
		modelHomR_sinEscala = glm::rotate(modelHomR_sinEscala, glm::radians(perArmR), glm::vec3(1.0f, 0.0f, 0.0f));

		// 8. Antebrazo Derecho (hijo de Hombro Der)
		glm::mat4 modelAntR = modelHomR_sinEscala;
		modelAntR = glm::translate(modelAntR, glm::vec3(0.0f, -0.0f, -0.50f));
		modelAntR = glm::rotate(modelAntR, glm::radians(perElbowR), glm::vec3(1.0f, 0.0f, 0.0f));
		modelAntR = glm::scale(modelAntR, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAntR));
		Pantebrasder.Draw(lightingShader);

		// 4.b. Dibujar Hombro Derecho (Padre)
		glm::mat4 modelHomR_conEscala = modelHomR_sinEscala;
		modelHomR_conEscala = glm::scale(modelHomR_conEscala, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHomR_conEscala));
		Phombroder.Draw(lightingShader);

		// --- JERARQUÍA DE LA PIERNA IZQUIERDA ---
		// 5. Muslo Izquierdo (Matriz padre para la rodilla)
		glm::mat4 modelMusL_sinEscala = personaBase;
		modelMusL_sinEscala = glm::translate(modelMusL_sinEscala, glm::vec3(0.0f, 0.30f, 0.0f));
		modelMusL_sinEscala = glm::rotate(modelMusL_sinEscala, glm::radians(perLegL), glm::vec3(1.0f, 0.0f, 0.0f));

		// 9. Rodilla Izquierda (hija de Muslo Izq)
		glm::mat4 modelRodL = modelMusL_sinEscala;
		modelRodL = glm::translate(modelRodL, glm::vec3(0.0f, 0.350f, 0.0f));
		modelRodL = glm::rotate(modelRodL, glm::radians(perKneeL), glm::vec3(1.0f, 0.0f, 0.0f));
		modelRodL = glm::scale(modelRodL, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRodL));
		Prodillaizq.Draw(lightingShader);

		// 5.b. Dibujar Muslo Izquierdo (Padre)
		glm::mat4 modelMusL_conEscala = modelMusL_sinEscala;
		modelMusL_conEscala = glm::scale(modelMusL_conEscala, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMusL_conEscala));
		Pmusloizq.Draw(lightingShader);

		// --- JERARQUÍA DE LA PIERNA DERECHA ---
		// 6. Muslo Derecho (Matriz padre para la rodilla)
		glm::mat4 modelMusR_sinEscala = personaBase;
		modelMusR_sinEscala = glm::translate(modelMusR_sinEscala, glm::vec3(-0.0f, 0.30f, 0.0f));
		modelMusR_sinEscala = glm::rotate(modelMusR_sinEscala, glm::radians(perLegR), glm::vec3(1.0f, 0.0f, 0.0f));

		// 10. Rodilla Derecha (hija de Muslo Der)
		glm::mat4 modelRodR = modelMusR_sinEscala;
		modelRodR = glm::translate(modelRodR, glm::vec3(0.0f, 0.350f, 0.0f));
		modelRodR = glm::rotate(modelRodR, glm::radians(perKneeR), glm::vec3(1.0f, 0.0f, 0.0f));
		modelRodR = glm::scale(modelRodR, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRodR));
		Prodillader.Draw(lightingShader);

		// 6.b. Dibujar Muslo Derecho (Padre)
		glm::mat4 modelMusR_conEscala = modelMusR_sinEscala;
		modelMusR_conEscala = glm::scale(modelMusR_conEscala, glm::vec3(0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMusR_conEscala));
		Pmusloder.Draw(lightingShader);


		// ===============================================
		// ===== FIN DE PERSONA =====
		// ===============================================


		// Escultura torso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Vietor.Draw(lightingShader);

		// Escultura cabeza
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Viecab.Draw(lightingShader);

		// Escultura brazo izquierdo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Viebraizq.Draw(lightingShader);

		// Escultura brazo derecho
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Viebrader.Draw(lightingShader);

		// Escultura antebrazo izquierdo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Vieanbizq.Draw(lightingShader);

		// Escultura antebrazo derecho
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Vieanbder.Draw(lightingShader);

		// Escultura muslo izquierdo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Viemusizq.Draw(lightingShader);

		// Escultura muslo derecho
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Viemusder.Draw(lightingShader);

		// Escultura pierna izquierda
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Viepieizq.Draw(lightingShader);

		// Escultura pierna derecha
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Viepieder.Draw(lightingShader);

		// ===============================================
		// ===== ESQUELETO (BAILE) =====
		// ===============================================
		// 1. Matriz Base (Torso) - SIN ESCALA
		glm::mat4 esqueletoBase = glm::mat4(1.0f);
		esqueletoBase = glm::translate(esqueletoBase, glm::vec3(skelPosX, skelPosY, skelPosZ));
		esqueletoBase = glm::rotate(esqueletoBase, glm::radians(skelBaseRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		esqueletoBase = glm::rotate(esqueletoBase, glm::radians(danceRotY), glm::vec3(0.0f, 1.0f, 0.0f));

		// 2. Torso (se dibuja con la base)
		glm::mat4 modelEtorso = esqueletoBase;
		modelEtorso = glm::scale(modelEtorso, glm::vec3(0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEtorso)); // <-- CORREGIDO
		Etorso.Draw(lightingShader); // <-- CORREGIDO

		// 3. Cabeza (hija de la base)
		glm::mat4 modelEcabeza = esqueletoBase;
		modelEcabeza = glm::scale(modelEcabeza, glm::vec3(0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEcabeza)); // <-- CORREGIDO
		Ecabeza.Draw(lightingShader); // <-- CORREGIDO

		// 4. Brazo Derecho (hijo de la base)
		glm::mat4 modelEbraR = esqueletoBase;
		modelEbraR = glm::rotate(modelEbraR, glm::radians(skelArmR), glm::vec3(1.0f, 0.0f, 0.0f));
		modelEbraR = glm::scale(modelEbraR, glm::vec3(0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEbraR)); // <-- CORREGIDO
		Ebrader.Draw(lightingShader); // <-- CORREGIDO

		// 5. Brazo Izquierdo (hijo de la base)
		glm::mat4 modelEbraL = esqueletoBase;
		modelEbraL = glm::rotate(modelEbraL, glm::radians(skelArmL), glm::vec3(1.0f, 0.0f, 0.0f));
		modelEbraL = glm::scale(modelEbraL, glm::vec3(0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEbraL)); // <-- CORREGIDO
		Ebraizq.Draw(lightingShader); // <-- CORREGIDO

		// 6. Pelvis (hija de la base y PADRE de las piernas)
		glm::mat4 modelEpelvis_sinEscala = esqueletoBase;

		// 6b. Dibujar Pelvis
		glm::mat4 modelEpelvis_conEscala = modelEpelvis_sinEscala;
		modelEpelvis_conEscala = glm::scale(modelEpelvis_conEscala, glm::vec3(0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEpelvis_conEscala)); // <-- CORREGIDO
		Epelvis.Draw(lightingShader); // <-- CORREGIDO

		// 7. Pierna Derecha (hija de Pelvis)
		glm::mat4 modelEpieR = modelEpelvis_sinEscala;
		modelEpieR = glm::rotate(modelEpieR, glm::radians(skelLegR), glm::vec3(1.0f, 0.0f, 0.0f));
		modelEpieR = glm::scale(modelEpieR, glm::vec3(0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEpieR)); // <-- CORREGIDO
		Epieder.Draw(lightingShader); // <-- CORREGIDO

		// 8. Pierna Izquierda (hija de Pelvis)
		glm::mat4 modelEpieL = modelEpelvis_sinEscala;
		modelEpieL = glm::rotate(modelEpieL, glm::radians(skelLegL), glm::vec3(1.0f, 0.0f, 0.0f));
		modelEpieL = glm::scale(modelEpieL, glm::vec3(0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEpieL)); // <-- CORREGIDO
		Epieizq.Draw(lightingShader); // <-- CORREGIDO

		// ===============================================
		// ===== FIN DE ESQUELETO =====
		// ===============================================


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Ball.Draw(lightingShader);

		// Restaura configuración normal
		glDisable(GL_BLEND);

		glBindVertexArray(0);


		// ###########################################################
		// ### DIBUJADO DE LA LÁMPARA (LAMP SHADER) ###
		// ###########################################################
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

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// ###########################################################
		// ### DIBUJADO DEL SKYBOX (SKYBOX SHADER) ###
		// ###########################################################
		// ### SKYBOX ###
		glDisable(GL_CULL_FACE);

		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);                 // no escribir al z-buffer

		skyboxshader.Use();
		glUniform1i(glGetUniformLocation(skyboxshader.Program, "skybox"), 0);
		glm::mat4 viewNoPos = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewNoPos));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// <<< nuevo: factor de oscuridad del skybox >>>
		glUniform1f(glGetUniformLocation(skyboxshader.Program, "uDarken"), 0.35f); // prueba 0.3–0.6


		glActiveTexture(GL_TEXTURE0);          // usa la unidad 0
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		glBindVertexArray(skyBoxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthMask(GL_TRUE);                  // restaura z-buffer
		glDepthFunc(GL_LESS);



		// Swap the buffers

		glfwSwapBuffers(window);

	}
	/////////////////Borramos buffers/////////////////
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyBoxVAO);
	glDeleteBuffers(1, &skyBoxVBO);
	ma_sound_uninit(&bgm_sound);
	ma_engine_uninit(&audio_engine);
	ma_sound_uninit(&steps_sound);
	ma_sound_uninit(&ladrido_sound);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Dog Controls
	if (keys[GLFW_KEY_1])
	{
		angle += speed;  // Incrementa el ángulo para el movimiento circular
		//dogPosX = radio * cos(angle); // Posición en X
		//dogPosZ = radio * sin(angle); // Posición en Z
		rotDog += 0.05f;
		FLegs = 15.0f * sin(rotDog * 0.05f); // Mueve las patas delanteras
		RLegs = 15.0f * sin(rotDog * 0.05f);  // Mueve las patas traseras
		// Mantener circularAngle en el rango [0, 2π]
		if (angle > 2 * 3.15) {
			angle -= 2 * 3.15;
		}
	}

	/*if (keys[GLFW_KEY_2]) rotDogX += 0.1f;
	if (keys[GLFW_KEY_3]) rotDogX -= 0.1f;

	if (keys[GLFW_KEY_4]) head += 0.1f;
	if (keys[GLFW_KEY_5]) head -= 0.1f;

	if (keys[GLFW_KEY_6]) tail += 0.1f;
	if (keys[GLFW_KEY_7]) tail -= 0.1f;

	if (keys[GLFW_KEY_8]) RLegs += 0.1f;
	if (keys[GLFW_KEY_9]) RLegs -= 0.1f;

	if (keys[GLFW_KEY_Z]) FLegsL += 0.1f;
	if (keys[GLFW_KEY_X]) FLegsL -= 0.1f;

	if (keys[GLFW_KEY_M]) FLegsR += 0.1f;
	if (keys[GLFW_KEY_N]) FLegsR -= 0.1f;

	if (keys[GLFW_KEY_H]) dogPosZ += 0.001;
	if (keys[GLFW_KEY_Y]) dogPosZ -= 0.001;

	if (keys[GLFW_KEY_G]) dogPosX -= 0.001;
	if (keys[GLFW_KEY_J]) dogPosX += 0.001;

	if (keys[GLFW_KEY_C]) dogPosY -= 0.001;
	if (keys[GLFW_KEY_V]) dogPosY += 0.001;*/

	//// ===== Controles PERSONA =====
	//if (keys[GLFW_KEY_1]) { // caminar simple con balanceo
	//	perRotY += 0.05f;
	//	float osc = 15.0f * sin(perRotY * 0.05f);
	//	perArmL = osc;
	//	perArmR = -osc;
	//	perLegL = -osc;
	//	perLegR = osc;
	//}

	//if (keys[GLFW_KEY_2]) perRotX += 0.1f;
	//if (keys[GLFW_KEY_3]) perRotX -= 0.1f;

	//if (keys[GLFW_KEY_4]) perHead += 0.1f;
	//if (keys[GLFW_KEY_5]) perHead -= 0.1f;

	//if (keys[GLFW_KEY_Z]) perArmL += 0.1f;
	//if (keys[GLFW_KEY_X]) perArmL -= 0.1f;

	//if (keys[GLFW_KEY_M]) perArmR += 0.1f;
	//if (keys[GLFW_KEY_N]) perArmR -= 0.1f;

	//// piernas (ajústales teclas si quieres independientes)
	//if (keys[GLFW_KEY_8]) perLegL += 0.1f;
	//if (keys[GLFW_KEY_9]) perLegL -= 0.1f;

	//if (keys[GLFW_KEY_H]) perPosZ += 0.001f;
	//if (keys[GLFW_KEY_Y]) perPosZ -= 0.001f;
	//if (keys[GLFW_KEY_G]) perPosX -= 0.001f;
	//if (keys[GLFW_KEY_J]) perPosX += 0.001f;
	//if (keys[GLFW_KEY_C]) perPosY -= 0.001f;
	//if (keys[GLFW_KEY_V]) perPosY += 0.001f;


	// Camera controls
	bool isMoving = false;
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
		isMoving = true;
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		isMoving = true;
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
		isMoving = true;
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
		isMoving = true;
	}

	// --- Lógica de Audio de Pasos ---
	if (isMoving)
	{
		// Si nos estamos moviendo y el sonido NO está sonando, lo iniciamos
		if (!ma_sound_is_playing(&steps_sound))
		{
			ma_sound_start(&steps_sound);
		}
	}
	else
	{
		// Si NO nos estamos moviendo y el sonido SÍ está sonando, lo detenemos
		if (ma_sound_is_playing(&steps_sound))
		{
			ma_sound_stop(&steps_sound);
		}
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
	camera.SetPositionY(4.0f);
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
	if (key == GLFW_KEY_U && GLFW_PRESS == action)
	{
		setupDogAnimation(); // 1. Cargar los keyframes

		// 2. Preparar para reproducir
		play = false;
		playIndex = 0;
		i_curr_steps = 0;

		// 3. RESET COMPLETO DE VARIABLES
		dogPosX = KeyFrame[0].dogPosX;
		dogPosY = KeyFrame[0].dogPosY;
		dogPosZ = KeyFrame[0].dogPosZ;
		rotDog = KeyFrame[0].rotDog;
		rotDogX = KeyFrame[0].rotDogX;

		FLegsL = KeyFrame[0].FLegsL;
		FLegsR = KeyFrame[0].FLegsR;
		RLegs = KeyFrame[0].RLegs;
		head = KeyFrame[0].head; // <-- AÑADIDO
		tail = KeyFrame[0].tail; // <-- AÑADIDO
		// ---------------------------

		interpolation(); // 4. Calcular primera interpolación
		play = true;     // 5. Iniciar
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
// ==========================================================
// --- MÁQUINA DE ESTADOS DE LA PERSONA (RUTA COMPLETA) ---
// ==========================================================
void AnimatePerson()
{
	// 's' es el seno de la fase, va de -1 a 1
	float s = 0.0f;

	switch (personState)
	{
	case WALK_Z_NEG: // TRAMO 1: Caminar 40 en -Z
	{
		// 1. Mover el cuerpo
		perPosZ -= personWalkSpeed * deltaTime;
		perPosY = 0.3f + 0.02f * (1.0f - cosf(2.0f * personWalkAngle));

		// 2. Animar el ciclo de caminata
		personWalkAngle += glm::two_pi<float>() * stepRate * deltaTime;
		if (personWalkAngle > glm::two_pi<float>()) personWalkAngle -= glm::two_pi<float>();
		s = sinf(personWalkAngle); // Seno de la fase

		// 3. Comprobar si ha llegado al destino
		if (perPosZ <= personTargetZ_1) {
			perPosZ = personTargetZ_1; // Snap
			personState = TURN_TO_X;   // Siguiente estado
			s = 0.0f; perPosY = 0.3f;
		}
		break;
	}

	case TURN_TO_X: // TRAMO 2: Girar 90 izq (180 -> 270)
	{
		perRotY += personTurnSpeed * deltaTime; // Giro anti-horario
		if (perRotY >= personTargetRot_1) {
			perRotY = personTargetRot_1;
			personState = WALK_X_NEG;
		}
		s = 0.0f;
		break;
	}

	case WALK_X_NEG: // TRAMO 3: Caminar 40 en -X
	{
		perPosX -= personWalkSpeed * deltaTime;
		perPosY = 0.3f + 0.02f * (1.0f - cosf(2.0f * personWalkAngle));
		personWalkAngle += glm::two_pi<float>() * stepRate * deltaTime;
		if (personWalkAngle > glm::two_pi<float>()) personWalkAngle -= glm::two_pi<float>();
		s = sinf(personWalkAngle);

		if (perPosX <= personTargetX_1) {
			perPosX = personTargetX_1;
			personState = TURN_180_X;
			s = 0.0f; perPosY = 0.3f;
		}
		break;
	}

	case TURN_180_X: // TRAMO 4: Gira 180 (270 -> 90)
	{
		perRotY -= personTurnSpeed * deltaTime; // Giro horario
		if (perRotY <= personTargetRot_2) {
			perRotY = personTargetRot_2;
			personState = WALK_X_POS;
		}
		s = 0.0f;
		break;
	}

	case WALK_X_POS: // TRAMO 5: Caminar 40 en +X (Regreso)
	{
		perPosX += personWalkSpeed * deltaTime;
		perPosY = 0.3f + 0.02f * (1.0f - cosf(2.0f * personWalkAngle));
		personWalkAngle += glm::two_pi<float>() * stepRate * deltaTime;
		if (personWalkAngle > glm::two_pi<float>()) personWalkAngle -= glm::two_pi<float>();
		s = sinf(personWalkAngle);

		if (perPosX >= personTargetX_2) {
			perPosX = personTargetX_2;
			personState = TURN_TO_Z;
			s = 0.0f; perPosY = 0.3f;
		}
		break;
	}

	case TURN_TO_Z: // TRAMO 6: Gira 90 der (90 -> 0)
	{
		perRotY -= personTurnSpeed * deltaTime; // Giro horario
		if (perRotY <= personTargetRot_3) {
			perRotY = 0.0f; // Snap a 0
			personState = WALK_Z_POS;
		}
		s = 0.0f;
		break;
	}

	case WALK_Z_POS: // TRAMO 7: Caminar 40 en +Z (Regreso)
	{
		perPosZ += personWalkSpeed * deltaTime;
		perPosY = 0.3f + 0.02f * (1.0f - cosf(2.0f * personWalkAngle));
		personWalkAngle += glm::two_pi<float>() * stepRate * deltaTime;
		if (personWalkAngle > glm::two_pi<float>()) personWalkAngle -= glm::two_pi<float>();
		s = sinf(personWalkAngle);

		if (perPosZ >= personTargetZ_2) {
			perPosZ = personTargetZ_2;
			personState = TURN_180_Z;
			s = 0.0f; perPosY = 0.3f;
		}
		break;
	}

	case TURN_180_Z: // TRAMO 8: Gira 180 (0 -> 180)
	{
		perRotY += personTurnSpeed * deltaTime; // Giro anti-horario
		if (perRotY >= personTargetRot_4) {
			perRotY = personTargetRot_4;
			personState = WALK_Z_NEG; // Reinicia el ciclo
		}
		s = 0.0f;
		break;
	}
	} // Fin del switch

	// --- Aplicar Animación de Articulaciones (TUS VALORES) ---
	// Caderas (Muslos)
	perLegL = 3.0f * s;
	perLegR = -3.0f * s;

	// Rodillas
	perKneeL = 2.0f + 8.0f * std::max(0.0f, s);
	perKneeR = 2.0f + 8.0f * std::max(0.0f, -s);

	// Hombros (Brazos)
	perArmL = -3.0f * s;
	perArmR = 3.0f * s;

	// Codos
	perElbowL = 8.0f;
	perElbowR = 8.0f;
}

void Animation() {

	// --- LÓGICA DE AUDIO DEL PERRO ---
	if (play && !ma_sound_is_playing(&ladrido_sound)) {
		// Si la animación debe estar activa y el sonido no está sonando, inícialo.
		ma_sound_start(&ladrido_sound);
	}
	else if (!play && ma_sound_is_playing(&ladrido_sound)) {
		// Si la animación está detenida y el sonido sigue, páralo.
		ma_sound_stop(&ladrido_sound);
	}
	// ---------------------------------


	if (play) {
		if (i_curr_steps >= i_max_steps) { // Fin del frame actual
			playIndex++;
			if (playIndex > FrameIndex - 2) { // Fin de toda la animación
				printf("Fin de animacion del perro.\n");
				play = false;
				playIndex = 0;
				// Ya no necesitamos parar el audio aquí, el check del siguiente frame lo hará.
			}
			else { // Siguiente frame
				i_curr_steps = 0;
				interpolation(); // Calculamos nuevos incrementos
			}
		}
		else {
			// --- Mover al PERRO ---
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;

			rotDog += KeyFrame[playIndex].rotDogInc;
			rotDogX += KeyFrame[playIndex].rotDogXInc;

			FLegsL += KeyFrame[playIndex].FLegsLInc;
			FLegsR += KeyFrame[playIndex].FLegsRInc;
			RLegs += KeyFrame[playIndex].RLegsInc;

			head += KeyFrame[playIndex].headInc;
			tail += KeyFrame[playIndex].tailInc;

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
// ==========================================================
// --- MÁQUINA DE ESTADOS DEL ESQUELETO (BAILE) ---
// ==========================================================
void AnimateSkeleton()
{
	// 1. Aumentar el "reloj" del baile y el giro
	// (0.8f es la velocidad del baile, 30.0f es la velocidad de giro)
	danceAngle += glm::two_pi<float>() * 0.8f * deltaTime;
	danceRotY += 30.0f * deltaTime; // Giro lento y constante

	// 2. Calcular oscilaciones simples (un baile "sencillo")
	float armSwing = 5.0f * sinf(danceAngle); // Los brazos se mueven 50°
	float legSwing = 5.0f * cosf(danceAngle); // Las piernas 35°, fuera de fase

	// 3. Aplicar a las articulaciones
	skelArmL = armSwing;
	skelArmR = -armSwing; // Opuesto

	skelLegL = legSwing;
	skelLegR = -legSwing; // Opuesto
}