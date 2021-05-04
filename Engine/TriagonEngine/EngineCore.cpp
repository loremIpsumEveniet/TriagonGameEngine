#pragma once

#include "dependencies.h"

#include "ObjectMaker.h"
#include "ShaderMaker.h"
#include "TextureLoader.h"

const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT = 1080;

double DELTA_TIME = 0.0;
double LastFrameTime = 0.0;
double CURRENT_FRAME = 0.0;

struct Camera* MainCamera;

//Camera Coords
struct Camera {
public:
	double sensitivity = 0.1;
	double CameraSpeed = 20.5; 

	glm::vec3 CameraPosition = glm::vec3(0.0, 0.0, 0.0);

	glm::vec3 CameraForwardVector = glm::vec3(1.0, 0.0, 0.0); //I can't fix this so the camera's "target" must always be 1.0 0.0 0.0
	glm::vec3 CameraUpVerctor = glm::vec3(0.0, 1.0, 0.0);

	Camera(glm::vec3 InnitialPosition) {
		CameraPosition = InnitialPosition;
	}
private:
	///THIS IS NEEDED IF I WISH TO CALCULATE CAMERA MATRIX BY MYSELF///
	/*glm::vec3 CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f); //Current camera position, (World Space)

	glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //What the camera is pointing at
	glm::vec3 CameraPositiveZ = glm::normalize(CameraPosition - CameraTarget); //Reverce from the camera direction, or in other words the local Z coordinate of the camera

	glm::vec3 ZAxis = glm::vec3(0.0f, 1.0f, 0.0f); //What is the Z Axis vector
	glm::vec3 CameraPositiveX = glm::normalize(glm::cross(ZAxis, CameraPositiveZ));//Cross product gives us a 3d "perpendicular" vector to the two vector, or in other words the local X coordinate of the camera

	glm::vec3 CameraPositiveY = glm::cross(CameraPositiveZ, CameraPositiveX);//The local Y coordinate of the camera
	CameraPositiveY = glm::normalize(CameraPositiveY);
	*/
	///^^THIS IS NEEDED IF I WISH TO CALCULATE CAMERA MATRIX BY MYSELF^^///

};


void framebuffer_size_callback(GLFWwindow* window, int width, int height) { //Resize the viewport if the user resize the window (this function is called by GLFW)
	glViewport(0, 0, width, height);
}


void inline ProcessKeyboardInput(GLFWwindow* window, Camera* MainCamera) { //This fucntion is not called by GLFW but by me (its just a shotcut)

	float ActualCameraSpeed = (*MainCamera).CameraSpeed * DELTA_TIME;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		printf("Pressed Escape\n");
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe?
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		(*MainCamera).CameraPosition += (*MainCamera).CameraForwardVector * ActualCameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		(*MainCamera).CameraPosition -= (*MainCamera).CameraForwardVector * ActualCameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		(*MainCamera).CameraPosition -= glm::normalize(glm::cross((*MainCamera).CameraForwardVector, (*MainCamera).CameraUpVerctor)) * ActualCameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		(*MainCamera).CameraPosition += glm::normalize(glm::cross((*MainCamera).CameraForwardVector, (*MainCamera).CameraUpVerctor)) * ActualCameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		(*MainCamera).CameraPosition += (*MainCamera).CameraUpVerctor * ActualCameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		(*MainCamera).CameraPosition -= (*MainCamera).CameraUpVerctor * ActualCameraSpeed;
	}
	///^^Camera Movement^^///
}

void ProcessMouseInput(GLFWwindow* window, double ScreenX, double ScreenY) {

	if (GLFW_FOCUSED) { //Check if the mouse is in the window

		static double prevScreenX = ScreenX;
		static double prevScreenY = ScreenY;


		static double DeltaRotationX = 0;
		static double DeltaRotationY = 0;

		///Camera Rotation///

		DeltaRotationX += (*MainCamera).sensitivity * (ScreenX - prevScreenX); //Camera yaw calculation

		DeltaRotationY += (*MainCamera).sensitivity * (prevScreenY - ScreenY);//Camera pitch calculation
		DeltaRotationY = DeltaRotationY > 89.0 ? 89.0 : DeltaRotationY; //This is nessasary so that we can't rotate our camera all the way around when looking up
		DeltaRotationY = DeltaRotationY < -89.0 ? -89.0 : DeltaRotationY;

		glm::vec3 NewCameraFowardVector;
		NewCameraFowardVector.x = cos(glm::radians(DeltaRotationX)) * cos(glm::radians(DeltaRotationY));
		NewCameraFowardVector.y = sin(glm::radians(DeltaRotationY));

		NewCameraFowardVector.z = sin(glm::radians(DeltaRotationX)) * cos(glm::radians(DeltaRotationY));
		(*MainCamera).CameraForwardVector = glm::normalize(NewCameraFowardVector);

		prevScreenX = ScreenX;
		prevScreenY = ScreenY;

		int X = 0;
	}
	
	///^^Camera Rotation^^///
}


void inline ClearBuffers(GLFWwindow* window) {

	glClearColor(0.75f, 0.75f, 0.75f, 1.0f); //Clear the screen from the last render //Also this is Backround color
	glClear(GL_COLOR_BUFFER_BIT); //Clear the Color Buffer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear The Z Buffer or depth information to be filled by new depth information
}

int main() {

	if (!glfwInit()) {
		// Initialization failed
		printf("Initialization Failed ");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* MainWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Engine", NULL, NULL);

	if (MainWindow == NULL) { //Error handling for initialization of the main window
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(MainWindow);
	glfwSetFramebufferSizeCallback(MainWindow, framebuffer_size_callback);

	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Kill cursor in the window
	glfwSetCursorPosCallback(MainWindow, ProcessMouseInput); //Send mouse movement information to the ProcessMouseInput function

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //Error handling for initialization of whatever glad is supposed to be
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	MainCamera = new Camera(glm::vec3(0.0, 0.0, 0.0)); //Innit the main camera

	/// Compiling Shaders From Source Files // //For Context Check "ShaderMaker" ///

	Shader SimpleShader = Shader("Shaders/BasicVertexShaderSource.glsl", "Shaders/BasicFragmentShaderSource.glsl");
	Shader InstanceShader = Shader("Shaders/InstanceVertexShaderSource.glsl", "Shaders/InstanceFragmentShaderSource.glsl");

	///^^Compiling Shaders From Source Files ^^///

	int numberOf_InstancedPositions = 160000;
	int counter = 0;

	double* InstancedPositions = (double*)malloc(sizeof(double) * 3 * numberOf_InstancedPositions);
	for (int i = 0; i < sqrt(numberOf_InstancedPositions); i++) {
		for (int j = 0; j < sqrt(numberOf_InstancedPositions); j++) {
			InstancedPositions[counter] = i;
			InstancedPositions[counter+1] = 0;
			InstancedPositions[counter+2] = j;
			counter+=3;
		}
	}

	unsigned int texture = LoadTexture("Textures/SergeyEdited.jpg", false);
	unsigned int texture2 = LoadTexture("Textures/Discord_Sucks.png", true);

	glActiveTexture(GL_TEXTURE0); //There are 16 guaranteed texture slots in OpenGl, GL_TEXTURE0 throught GL_TEXTURE15
	glBindTexture(GL_TEXTURE_2D, texture);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture2);

	SimpleShader.setInt("Texture2", 1); //This Setts what GPU texture slot to use, the are all in order from GL_TEXTURE0 ot GL_TEXTURE16, and GL_TEXTURE0 + 8 = GL_TEXTURE8

	Object Bob("Models/TestCubeTiny.obj", "TestCube",SimpleShader);

	Object John("Models/TestCubeTiny.obj", "TestCube2", InstancedPositions, numberOf_InstancedPositions,InstanceShader);

	Object SimplePlane("Models/SimplePlane.obj", "TestCube", SimpleShader);

	glEnable(GL_DEPTH_TEST); //Eneable opengl to depth test before drawing, only needs to be done once, unless we need to disable it (for some reason)

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //Back Face Culling

	while (!glfwWindowShouldClose(MainWindow)) { //This is the MAIN Render Loop

		///Complete Last Frame Operations///

		ClearBuffers(MainWindow);

		///^^Complete Last Frame Operations^^///


		/// Poll for and process events ///

		ProcessKeyboardInput(MainWindow, MainCamera); //get input from keyboard

		///^^ Poll for and process events ^^///
	

		///Time Related Calculations///

		CURRENT_FRAME = glfwGetTime(); //Don't ever use this, only use LastFrameTime Or Delta_Time
		DELTA_TIME = CURRENT_FRAME - LastFrameTime;
		LastFrameTime = CURRENT_FRAME;

		///^^Time Related Calculations^^///


		///Camera Matrix Calculation///

		glm::mat4 CameraMatrix = glm::lookAt((*MainCamera).CameraPosition, (*MainCamera).CameraPosition + (*MainCamera).CameraForwardVector, (*MainCamera).CameraUpVerctor);
		
		///^^^Camera Matrix Calculation^^^///


		/// Render here ///

		Bob.Draw(glm::vec3(3,2,0),CameraMatrix);
		SimplePlane.Draw(glm::vec3(0, -1, 0), CameraMatrix);

		John.DrawStaticInstanced(glm::vec3(-100, 1, -100), CameraMatrix);

		///^^ Render here ^^ ///

		glfwSwapBuffers(MainWindow); //GLFW/OpenGl Renders to A "Second Screen"/Buffer then swaps the two to remove any rendering artifacts
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}