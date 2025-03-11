//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//
#define GLM_ENABLE_EXPERIMENTAL
#if defined (__APPLE__)
	#define GLFW_INCLUDE_GLCOREARB
	#define GL_SILENCE_DEPRECATION
#else
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"

#include <iostream>

int glWindowWidth = 800;
int glWindowHeight = 600;
int SHADOW_WIDTH = 800;
int SHADOW_HEIGHT = 600;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

glm::mat4 model;
GLuint modelLoc;
GLuint modelLoc2;


glm::mat4 view;
GLuint viewLoc;
GLuint viewLoc2;
glm::mat4 projection;
GLuint projectionLoc;
GLuint projectionLoc2;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;

glm::vec3 lightDirLiniar;
GLuint lightDirLiniarLoc;
glm::vec3 lightColorLiniar;
GLuint lightColorLiniarLoc;

glm::vec3 lightDirPunct;
GLuint lightDirPunctLoc;
glm::vec3 lightColorPunct;
GLuint lightColorPunctLoc;
glm::mat4 lightSpaceTrMatrix;
GLuint lightSpaceTrMatrixLoc;

int mod = 0;
GLuint shadowMapFBO;

glm::vec3 lightPosEye;
GLuint lightPosEyeLoc;

glm::vec3 lightPosR1;
GLuint lightPosR1Loc;

glm::vec3 lightPosR2;
GLuint lightPosR2Loc;

glm::vec3 lightPosB1;
GLuint lightPosB1Loc;

glm::vec3 lightPosB2;
GLuint lightPosB2Loc;

int umbre;
GLuint umbreLoc;

int stareLumina;
GLuint stareLuminaLoc;
int luminaCount = 0;


float alpha;
float alphaLoc;
gps::Camera myCamera(
				glm::vec3(0.0f, 0.0f, 2.5f), 
				glm::vec3(0.0f, 0.0f, -10.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 0.1f;

bool pressedKeys[1024];
float angleY = 0.0f;

gps::Model3D myModel;
gps::Model3D myModelElMare;
gps::Model3D myModelElMica;
gps::Model3D picPloaie;
gps::Model3D windows;

gps::Shader myCustomShader;
gps::Shader shadowShader;
gps::Shader shaderPloaie;

std::vector<glm::vec3> points = {
	glm::vec3(0, 1, 0),
	glm::vec3(1, 1, -11),
	glm::vec3(0, 1, -15),
	glm::vec3(-2, 1, -20),
	glm::vec3(-3, 1, -12),
	glm::vec3(11, 1, -3),
	glm::vec3(17, 12, -4),
	glm::vec3(32, 12, -6),
	glm::vec3(32, 12, 0),
	glm::vec3(0, 1, 0)
};
std::vector<glm::vec3> looksPoints = {
	glm::vec3(-0.6, 1, -12),
	glm::vec3(-0.6, 0.7, -12),
	glm::vec3(-0.6, 0.7, -12),
	glm::vec3(-1, 0.7, -16),
	glm::vec3(-1, 1, -16),
	glm::vec3(14, 1, -5),
	glm::vec3(23.7, 12, -3),
	glm::vec3(23.7, 12, -3),
	glm::vec3(23.7, 12, -3),
	glm::vec3(0, 1, 0)
};
int nrPoints = points.size();
int curentPoint = 0;


int actPloaie = 0;
bool keyPressedX = false;
bool keyPressedZ = false;
bool keyPressedF = false;
bool keyPressedP = false;
bool keyPressedC = false;
bool keyPressedT = false;
GLenum glCheckError_(const char *file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
	if (action == GLFW_RELEASE) {
		keyPressedX = false;
		keyPressedF = false;
		keyPressedZ = false;
		keyPressedP = false;
		keyPressedC = false;
		keyPressedT = false;
	}
}

double lastxpos = 0, lastypos = 0;
bool firstMouse = true;
float lastX = glWindowWidth / 2;
float lastY = glWindowHeight / 2;
float yaw = -90.0f;
float pitch = 0.0f;

int lumina1;
GLuint lumina1Loc;
int lumina2;
GLuint lumina2Loc;
GLuint depthMapTexture;
int ceata=0;
GLuint ceataLoc;
float transp;
GLuint transpLoc;
int autoMove = 0;
int nrPic = 1000;

struct Picaturi {
	glm::vec3 pos;  
	float viteza;
	float life;          
};
std::vector<Picaturi> ploaie;

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	autoMove = 0;
	static bool firstMouse = true;
	static float lastX = 0.0f;
	static float lastY = 0.0f;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; 
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	myCamera.rotate(pitch, yaw);
	glm::mat4 view = myCamera.getViewMatrix();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}
float a = 0.0f;
void autoMoveCamera() {
	glm::vec3 currentPosition = myCamera.getCameraPosition();
	glm::vec3 target = points[curentPoint];

	if (length(currentPosition - target) < 0.1f) {
		curentPoint++;
		a = 0;
		if (curentPoint == nrPoints) {
			curentPoint = 0;
		}
		return;
	}
    if (a < 1) {
	    a += 0.0001f;
     }
	glm::vec3 direction = glm::normalize(target - currentPosition);
	myCamera.moveInDirection(direction, looksPoints[curentPoint], 0.01f, a);
	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}


void processMovement()
{
	if (pressedKeys[GLFW_KEY_C] && !keyPressedC) {
		keyPressedC = true;
		ceata = 1 - ceata;
		glUniform1i(ceataLoc, ceata);
	}
	if (pressedKeys[GLFW_KEY_T] && !keyPressedT) {

		keyPressedT = true;
		autoMove = 1 - autoMove;
	}
	if (pressedKeys[GLFW_KEY_X]&&!keyPressedX) {
		keyPressedX = true;
		lumina1 = 1 - lumina1;
		glUniform1i(lumina1Loc, lumina1);
	}
	if (pressedKeys[GLFW_KEY_P] && !keyPressedP) {
		keyPressedP = true;
		actPloaie = 1 - actPloaie;
	}
	if (pressedKeys[GLFW_KEY_Z] && !keyPressedZ) {
		keyPressedZ = true;
		lumina2 = 1 - lumina2;
		glUniform1i(lumina2Loc, lumina2);
	}
	if (pressedKeys[GLFW_KEY_F] && !keyPressedF) {
		keyPressedF = true;
		mod = (mod + 1) % 3;
	}
	if (pressedKeys[GLFW_KEY_Q]) {
		angleY -= 1.0f;
		model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angleY += 1.0f;
		model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	int ok = 0;
	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
		autoMove = 0;
		ok = 1;
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
		ok = 1;
		autoMove = 0;
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
		ok = 1;
		autoMove = 0;
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
		ok = 1;
		autoMove = 0;
	}
	if (ok) {
		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	}
}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//window scaling for HiDPI displays
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	//for sRBG framebuffer
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

	//for antialising
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

#if not defined (__APPLE__)
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);

	glGenFramebuffers(1, &shadowMapFBO);
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture,0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void drawObjects(gps::Shader shader, bool depthPass) {

	shader.useShaderProgram();

	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	myModel.Draw(shader);
}
void initObjects() {
	myModel.LoadModel("objects/scenaOBJ.obj");
	myModelElMare.LoadModel("objects/eliceMare.obj");
	myModelElMica.LoadModel("objects/eliceMica.obj");
	picPloaie.LoadModel("objects/picaturiPloaie.obj");
	windows.LoadModel("objects/windows.obj");

	for (int i = 0; i < nrPic; i++) {
		Picaturi p;
		p.pos = glm::vec3((rand() % 60) - 30, (rand() % 5) + 10, (rand() % 60) - 30);
		p.viteza = (float) (rand() % 5) / 50.0;
		p.life = 1.0f;  
		ploaie.push_back(p);
	}

}
void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	shadowShader.loadShader("shaders/shaderShadow.vert", "shaders/shaderShadow.frag");
	//shaderPloaie.loadShader("shaders/shaderPloaie.vert", "shaders/shaderPloaie.frag");
	myCustomShader.useShaderProgram();
}

void initUniforms() {
	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	lightDirLiniar = glm::vec3(0.0f, -1.0f, 1.0f);
	lightDirLiniarLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDirLiniar");
	glUniform3fv(lightDirLiniarLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * lightDirLiniar));

	lightColorLiniar = glm::vec3(1.0f, 1.0f, 1.0f); 
	lightColorLiniarLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColorLiniar");
	glUniform3fv(lightColorLiniarLoc, 1, glm::value_ptr(lightColorLiniar));


	umbre =1;
	umbreLoc = glGetUniformLocation(myCustomShader.shaderProgram, "umbre");
	glUniform1i(umbreLoc, umbre);

	ceata = 0;
	ceataLoc = glGetUniformLocation(myCustomShader.shaderProgram, "ceata");
	glUniform1i(ceataLoc, ceata);

	transp = 1;
	transpLoc = glGetUniformLocation(myCustomShader.shaderProgram, "transp");
	glUniform1f(transpLoc, transp);

	model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	lightPosR1 = glm::vec3(23, 1 ,-3.5); 
	lightPosR2 = glm::vec3(18, 1, -4.5);
	lightPosB1 = glm::vec3(23, 1, -4);
	lightPosB2 = glm::vec3(18, 1, -5);



	lightPosR1Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosR1");
	lightPosR2Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosR2");
	lightPosB1Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosB1");
	lightPosB2Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosB2");
	glUniform3fv(lightPosR1Loc, 1, glm::value_ptr(lightPosR1));
	glUniform3fv(lightPosR2Loc, 1, glm::value_ptr(lightPosR2));
	glUniform3fv(lightPosB1Loc, 1, glm::value_ptr(lightPosB1));
	glUniform3fv(lightPosB2Loc, 1, glm::value_ptr(lightPosB2));

	stareLumina = 0;
	stareLuminaLoc = glGetUniformLocation(myCustomShader.shaderProgram, "stareLumina");
	glUniform1i(stareLuminaLoc, stareLumina);

	lumina1 = 1;
	lumina1Loc=glGetUniformLocation(myCustomShader.shaderProgram, "lumina1");
	glUniform1i(lumina1Loc, lumina1);
	lumina2 = 1;
	lumina2Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lumina2");
	glUniform1i(lumina2Loc, lumina2);

	glm::mat4 lightView = glm::lookAt(lightDirLiniar, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = 0.1f, far_plane = 6.0f;
	glm::mat4 lightProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);
	lightSpaceTrMatrix = lightProjection * lightView;
	lightSpaceTrMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix");
	glUniform3fv(lightSpaceTrMatrixLoc, 1, glm::value_ptr(lightSpaceTrMatrix));


	shaderPloaie.useShaderProgram();
	modelLoc2 = glGetUniformLocation(shaderPloaie.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));
	viewLoc2 = glGetUniformLocation(shaderPloaie.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
	projectionLoc2 = glGetUniformLocation(shaderPloaie.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection));
	myCustomShader.useShaderProgram();
}
float angle = 0;

glm::mat4 computeLightSpaceTrMatrix() {
	lightDirLiniar = glm::vec3(sin(glm::radians(angleY)), 1.0f, cos(glm::radians(angleY)));
	glm::mat4 lightView = glm::lookAt(lightDirLiniar, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = -50.0f, far_plane = 50.0f;
	glm::mat4 lightProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
	return lightSpaceTrMatrix;
}
void renderScene() { 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (mod == 1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else if(mod==2){
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		 }


	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	myCustomShader.useShaderProgram();
	glm::mat4 lightSpaceTrMatrix = computeLightSpaceTrMatrix();
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceTrMatrix));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUniformMatrix4fv(glGetUniformLocation(shadowShader.shaderProgram, "lightSpaceTrMatrix"),1,GL_FALSE,glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	umbre = 1;
	glUniform1i(umbreLoc, umbre);
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	myModel.Draw(myCustomShader);
	transp = 0.9;
	glUniform1f(transpLoc, transp);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	windows.Draw(myCustomShader);
	transp = 1;
	glUniform1f(transpLoc, transp);
	myModel.Draw(myCustomShader);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(24.2, 13, -3.5));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	myModelElMare.Draw(myCustomShader);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(26.624, 12.563, -3.665));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	myModelElMica.Draw(myCustomShader);
	umbre = 0;
	glUniform1i(umbreLoc, umbre);
	if (actPloaie == 1) {
		for (auto& p : ploaie) {
				model = glm::mat4(1.0);
				model = glm::translate(model, p.pos);
				model = glm::scale(model, glm::vec3(0.7, 0.7, 0.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				float deltaTime = 0.005;
				picPloaie.Draw(myCustomShader);
				if (p.life > 0.0f) {
					p.pos.y += -p.viteza;
					p.life -= deltaTime;
				}
				else {
					p.pos = glm::vec3((rand()%60) - 30, (rand() % 30) , (rand() % 60) - 30);
					p.life = 1.0f;
				}
		}
	}
	if (autoMove == 1) {
		autoMoveCamera();
	}
	angle += 15;
	luminaCount++;
	if (luminaCount == 30) {
		stareLumina = 1 - stareLumina;
		luminaCount=0;
		glUniform1i(stareLuminaLoc, stareLumina);
	}
}

void cleanup() {

	glfwDestroyWindow(glWindow);
	glfwTerminate();
}
int main(int argc, const char * argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();



	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(glWindow)) {
		glfwPollEvents();
		processMovement();
		glfwSwapBuffers(glWindow);
		renderScene();
	}

	cleanup();

	return 0;
}
