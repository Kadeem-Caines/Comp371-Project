#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

//used for uploading music
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") 



#define GLEW_STATIC 1   
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 


#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp> 


#include <cstdlib>

#include <FreeImageIO.h>

#include "OBJloader.h"  //For loading .obj files
using namespace glm;





//initial positions for car parts
glm::vec3 body = glm::vec3(0.0, 4.0f, 0.0f);
glm::vec3 left_back_wheel = glm::vec3(5.0f, 1.0f, 3.0f);
glm::vec3 left_front_wheel = glm::vec3(-5.0f, 1.0f, 3.0f);
glm::vec3 right_front_wheel = glm::vec3(-5.0f, 1.0f, -3.0f);
glm::vec3 right_rear_wheel = glm::vec3(5.0f, 1.0f, -3.0f);
glm::vec3 trunk = glm::vec3(6.0f, 3.0f, 0.0f);
glm::vec3 roof = glm::vec3(1.5f, 7.0f, 0.0f);
glm::vec3 bonnet = glm::vec3(-6.0f, 3.0f, 0.0f);
glm::vec3 rotateOriginal = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 headlight1 = glm::vec3(-8.0f, 3.0f, 1.0f);
glm::vec3 headlight2 = glm::vec3(-8.0f, 3.0f, -1.0f);
glm::vec3 tailight1 = glm::vec3(7.0f, 3.0f, 2.0f);
glm::vec3 tailight2 = glm::vec3(7.0f, 3.0f, -2.0f);


//initial scaling for car parts
glm::vec3 body1 = glm::vec3(5.0f, 2.0f, 3.0f);
glm::vec3 left_back_wheel1 = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 left_front_wheel1 = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 right_front_wheel1 = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 right_rear_wheel1 = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 trunk1 = glm::vec3(1.0f, 1.0f, 3.0f);
glm::vec3 roof1 = glm::vec3(2.0f, 1.0f, 3.0f);
glm::vec3 bonnet1 = glm::vec3(2.0f, 1.0f, 2.0f);


//initial particle poosition
glm::vec3 particlePosition= glm::vec3(7.0f, 3.0f, 0.0f);

//used for rotating
GLfloat rotation1 = 1.0f;


glm::vec3 headlights(10.2f, 10.2f, 10.2f);


//used for turning the car textures on and off
GLuint goldTextureContainer;
GLuint tireTextureContainer;

//initial camera coordinates

glm::vec3 cameraPosition = glm::vec3(0.0f, 50.0f, -100.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);




//Window Dimensions
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;


// point light location
glm::vec3 lightPos(0.0f, 35.0f, 0.5f);



//control camera with arrow keys



//method containing camera controls depending on button presses 

void keyboardInput(GLFWwindow* window)
{
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float cameraSpeed = 1.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPosition += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraPosition -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	//T for triangles
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//P for points
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	//L for lines
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//Reset camera to original position
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
		cameraPosition = glm::vec3(0.0f, 50.0f, -100.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	}

	//back up the car

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

		body += glm::vec3(1.0f, 0.0f, 0.0f);
		left_back_wheel += glm::vec3(1.0f, 0.0f, 0.0f);
		left_front_wheel += glm::vec3(1.0f, 0.0f, 0.0f);
		right_front_wheel += glm::vec3(1.0f, 0.0f, 0.0f);
		right_rear_wheel += glm::vec3(1.0f, 0.0f, 0.0f);
		trunk += glm::vec3(1.0f, 0.0f, 0.0f);
		roof += glm::vec3(1.0f, 0.0f, 0.0f);
		bonnet += glm::vec3(1.0f, 0.0f, 0.0f);
		particlePosition+= glm::vec3(1.0f, 0.0f, 0.0f);
		headlight1 += glm::vec3(1.0f, 0.0f, 0.0f);
		headlight2 += glm::vec3(1.0f, 0.0f, 0.0f);
		tailight1 += glm::vec3(1.0f, 0.0f, 0.0f);
		tailight2 += glm::vec3(1.0f, 0.0f, 0.0f);
	}

	// move the car forward


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		body -= glm::vec3(1.0f, 0.0f, 0.0f);
		left_back_wheel -= glm::vec3(1.0f, 0.0f, 0.0f);
		left_front_wheel -= glm::vec3(1.0f, 0.0f, 0.0f);
		right_front_wheel -= glm::vec3(1.0f, 0.0f, 0.0f);
		right_rear_wheel -= glm::vec3(1.0f, 0.0f, 0.0f);
		trunk -= glm::vec3(1.0f, 0.0f, 0.0f);
		roof -= glm::vec3(1.0f, 0.0f, 0.0f);
		bonnet -= glm::vec3(1.0f, 0.0f, 0.0f);
		particlePosition -= glm::vec3(1.0f, 0.0f, 0.0f);
		headlight1 -= glm::vec3(1.0f, 0.0f, 0.0f);
		headlight2 -= glm::vec3(1.0f, 0.0f, 0.0f);
		 tailight1 -= glm::vec3(1.0f, 0.0f, 0.0f);
		tailight2 -= glm::vec3(1.0f, 0.0f, 0.0f);


	}


	//shift car to the right

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

		body += glm::vec3(0.0f, 0.0f, 1.0f);
		left_back_wheel += glm::vec3(0.0f, 0.0f, 1.0f);
		left_front_wheel += glm::vec3(0.0f, 0.0f, 1.0f);
		right_front_wheel += glm::vec3(0.0f, 0.0f, 1.0f);
		right_rear_wheel += glm::vec3(0.0f, 0.0f, 1.0f);
		trunk += glm::vec3(0.0f, 0.0f, 1.0f);
		roof += glm::vec3(0.0f, 0.0f, 1.0f);
		bonnet += glm::vec3(0.0f, 0.0f, 1.0f);
		particlePosition += glm::vec3(0.0f, 0.0f, 1.0f);
		headlight1 += glm::vec3(0.0f, 0.0f, 1.0f);
		headlight2 += glm::vec3(0.0f, 0.0f, 1.0f);
		tailight1 += glm::vec3(0.0f, 0.0f, 1.0f);
		tailight2 += glm::vec3(0.0f, 0.0f, 1.0f);

	}

	//shift car to the left

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		body -= glm::vec3(0.0f, 0.0f, 1.0f);
		left_back_wheel -= glm::vec3(0.0f, 0.0f, 1.0f);
		left_front_wheel -= glm::vec3(0.0f, 0.0f, 1.0f);
		right_front_wheel -= glm::vec3(0.0f, 0.0f, 1.0f);
		right_rear_wheel -= glm::vec3(0.0f, 0.0f, 1.0f);
		trunk -= glm::vec3(0.0f, 0.0f, 1.0f);
		roof -= glm::vec3(0.0f, 0.0f, 1.0f);
		bonnet -= glm::vec3(0.0f, 0.0f, 1.0f);
		particlePosition -= glm::vec3(0.0f, 0.0f, 1.0f);
		headlight1 -= glm::vec3(0.0f, 0.0f, 1.0f);
		headlight2 -= glm::vec3(0.0f, 0.0f, 1.0f);
		tailight1 -= glm::vec3(0.0f, 0.0f, 1.0f);
		tailight2 -= glm::vec3(0.0f, 0.0f, 1.0f);
	}

	//shift car diagonally down left

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {

		body += glm::vec3(1.0f, 0.0f, 1.0f);
		left_back_wheel += glm::vec3(1.0f, 0.0f, 1.0f);
		left_front_wheel += glm::vec3(1.0f, 0.0f, 1.0f);
		right_front_wheel += glm::vec3(1.0f, 0.0f, 1.0f);
		right_rear_wheel += glm::vec3(1.0f, 0.0f, 1.0f);
		trunk += glm::vec3(1.0f, 0.0f, 1.0f);
		roof += glm::vec3(1.0f, 0.0f, 1.0f);
		bonnet += glm::vec3(1.0f, 0.0f, 1.0f);
		particlePosition += glm::vec3(1.0f, 0.0f, 1.0f);
		headlight1 += glm::vec3(1.0f, 0.0f, 1.0f);
		headlight2 += glm::vec3(1.0f, 0.0f, 1.0f);
		tailight1 += glm::vec3(1.0f, 0.0f, 1.0f);
		tailight2 += glm::vec3(1.0f, 0.0f, 1.0f);
	}


	//shift car diagonally right

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

		body -= glm::vec3(1.0f, 0.0f, 1.0f);
		left_back_wheel -= glm::vec3(1.0f, 0.0f, 1.0f);
		left_front_wheel -= glm::vec3(1.0f, 0.0f, 1.0f);
		right_front_wheel -= glm::vec3(1.0f, 0.0f, 1.0f);
		right_rear_wheel -= glm::vec3(1.0f, 0.0f, 1.0f);
		trunk -= glm::vec3(1.0f, 0.0f, 1.0f);
		roof -= glm::vec3(1.0f, 0.0f, 1.0f);
		bonnet -= glm::vec3(1.0f, 0.0f, 1.0f);
		particlePosition -= glm::vec3(1.0f, 0.0f, 1.0f);
		headlight1 -= glm::vec3(1.0f, 0.0f, 1.0f);
		headlight2 -= glm::vec3(1.0f, 0.0f, 1.0f);
		tailight1 -= glm::vec3(1.0f, 0.0f, 1.0f);
		tailight2 -= glm::vec3(1.0f, 0.0f, 1.0f);

	}




	if (glfwGetKey(window, GLFW_KEY_F) != GLFW_RELEASE) {
		rotation1 += 1.0f;
	}

	//Scale up
	if (glfwGetKey(window, GLFW_KEY_U) != GLFW_RELEASE) {
		body1 += glm::vec3(0.1f, 0.1f, 0.1f);
		trunk1 += glm::vec3(0.1f, 0.1f, 0.1f);
		left_back_wheel1 += glm::vec3(0.1f, 0.1f, 0.1f);
		left_front_wheel1 += glm::vec3(0.1f, 0.1f, 0.1f);
		right_front_wheel1 += glm::vec3(0.1f, 0.1f, 0.1f);
		right_rear_wheel1 += glm::vec3(0.1f, 0.1f, 0.1f);

		roof1 += glm::vec3(0.1f, 0.1f, 0.1f);
		bonnet1 += glm::vec3(0.1f, 0.1f, 0.1f);

	}

	//Scale down
	if (glfwGetKey(window, GLFW_KEY_J) != GLFW_RELEASE) {
		body1 -= glm::vec3(0.1f, 0.1f, 0.1f);
		trunk1 -= glm::vec3(0.1f, 0.1f, 0.1f);
		left_back_wheel1 -= glm::vec3(0.1f, 0.1f, 0.1f);
		left_front_wheel1 -= glm::vec3(0.1f, 0.1f, 0.1f);
		right_front_wheel1 -= glm::vec3(0.1f, 0.1f, 0.1f);
		right_rear_wheel1 -= glm::vec3(0.1f, 0.1f, 0.1f);

		roof1 -= glm::vec3(0.1f, 0.1f, 0.1f);
		bonnet1 -= glm::vec3(0.1f, 0.1f, 0.1f);



		
	}



	//random space on grid
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {

		GLfloat X = 0.05f;
		GLfloat Y = 1.0f;

		GLfloat r = static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / X));
		r += static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / Y));

		body = glm::vec3(1.0f + r + 0.0f, 4.0f, r + 0.0f + 1.0f);
		left_back_wheel = glm::vec3(1.0f + r + 5.0f, 1.0f, r + 3.0f + 1.0f);
		left_front_wheel = glm::vec3(1.0f + r - 5.0f, 1.0f, r + 3.0f + 1.0f);
		right_front_wheel = glm::vec3(1.0f + r - 5.0f, 1.0f, r - 3.0f + 1.0f);
		right_rear_wheel = glm::vec3(1.0f + r + 5.0f, 1.0f, r - 3.0f + 1.0f);
		trunk = glm::vec3(1.0f + r + 6.0f, 3.0f, r + 0.0f + 1.0f);
		roof = glm::vec3(1.0f + r + 1.5f, 7.0f, r + 0.0f + 1.0f);
		bonnet = glm::vec3(1.0f + r - 6.0f, 3.0f, r + 0.0f + 1.0f);


	}



	//Resets car back to original position and size
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {


		body = glm::vec3(0.0, 4.0f, 0.0f);
		left_back_wheel = glm::vec3(5.0f, 1.0f, 3.0f);
		left_front_wheel = glm::vec3(-5.0f, 1.0f, 3.0f);
		right_front_wheel = glm::vec3(-5.0f, 1.0f, -3.0f);
		right_rear_wheel = glm::vec3(5.0f, 1.0f, -3.0f);
		trunk = glm::vec3(6.0f, 3.0f, 0.0f);
		roof = glm::vec3(1.5f, 7.0f, 0.0f);
		bonnet = glm::vec3(-6.0f, 3.0f, 0.0f);
		rotateOriginal = glm::vec3(1.0f, 0.0f, 0.0f);


		body1 = glm::vec3(5.0f, 2.0f, 3.0f);
		left_back_wheel1 = glm::vec3(1.0f, 1.0f, 1.0f);
		left_front_wheel1 = glm::vec3(1.0f, 1.0f, 1.0f);
		right_front_wheel1 = glm::vec3(1.0f, 1.0f, 1.0f);
		right_rear_wheel1 = glm::vec3(1.0f, 1.0f, 1.0f);
		trunk1 = glm::vec3(1.0f, 1.0f, 3.0f);
		roof1 = glm::vec3(2.0f, 1.0f, 3.0f);
		bonnet1 = glm::vec3(2.0f, 1.0f, 2.0f);
		headlight1 = glm::vec3(-8.0f, 3.0f, 1.0f);
		 headlight2 = glm::vec3(-8.0f, 3.0f, -1.0f);
		tailight1 = glm::vec3(7.0f, 3.0f, 2.0f);
		 tailight2 = glm::vec3(7.0f, 3.0f, -2.0f);

		 particlePosition = glm::vec3(3.0f, 0, 0.0f);
	
	}


	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {

		headlights= glm::vec3(0.0f, 0.0f, 0.0f);

	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {

	 headlights= glm::vec3(10.2f, 10.2f, 10.2f);

	}




}

//variables for mouse camera movement taken from: https://learnopengl.com/Getting-started/Camera
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;


//mouse controls taken from: https://learnopengl.com/Getting-started/Camera

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}



//Method for reading shader files, compiling then linking them. returns shaderprogram

int compileAndLinkShaders(string vertex_shader_path, string fragment_shader_path)
{

	string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

	// Read the vertex Shader code from the file
	if (VertexShaderStream.is_open()) {
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Not opening. Incorrect file path", vertex_shader_path.c_str());
		exit(-1);
	}

	// Read the Fragment Shader code from the file

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Not opening. Incorrect file path.", fragment_shader_path.c_str());
		exit(-1);
	}

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

//method to load in texture image , taken from lab04 (concordia comp 371 tutorial)
int loadTexture(char* imagepath)
{
	// Load image using the Free Image library
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(imagepath, 0);
	FIBITMAP* image = FreeImage_Load(format, imagepath);
	FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);

	// Get an available texture index from OpenGL
	GLuint texture = 1;
	glGenTextures(1, &texture);
	assert(texture != 0);

	// Set OpenGL filtering properties (bi-linear interpolation)
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Retrieve width and hight
	int width = FreeImage_GetWidth(image32bits);
	int height = FreeImage_GetHeight(image32bits);

	// This will upload the texture to the GPU memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));

	// Free images
	FreeImage_Unload(image);
	FreeImage_Unload(image32bits);

	return texture;
}

//method to load in obj files  , taken from lab06 (concordia comp 371 tutorial)
GLuint setupModelVBO(string path, int& vertexCount) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	//read the vertex data from the model's OBJ file
	loadOBJ(path.c_str(), vertices, normals, UVs);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //Becomes active VAO
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

	//Vertex VBO setup
	GLuint vertices_VBO;
	glGenBuffers(1, &vertices_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Normals VBO setup
	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//UVs VBO setup
	GLuint uvs_VBO;
	glGenBuffers(1, &uvs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs, as we are using multiple VAOs)
	vertexCount = vertices.size();
	return VAO;
}


struct TexturedColoredVertex
{
	TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
		: position(_position), color(_color), uv(_uv) {}

	vec3 position;
	vec3 color;
	vec2 uv;
};



struct Light {

	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};



void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
	glUseProgram(shaderProgram);
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

//used for shadow shader
void setlightSpaceMatrix(int shaderProgram, mat4 lightSpaceMatrix)
{
	glUseProgram(shaderProgram);
	GLuint lightMatrixLocation = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");
	glUniformMatrix4fv(lightMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
}


//The following particle code was taken from http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/#particles-lots-of-them-

struct Particle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};


const int MaxParticles = 100000;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;

//The following particle code was taken from http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/#particles-lots-of-them-

int FindUnusedParticle() {

	for (int i = LastUsedParticle; i < MaxParticles; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

//The following particle code was taken from http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/#particles-lots-of-them-

void SortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}


std::vector<glm::vec3> mControlPoints;

void AddControlPoint(glm::vec3 point)
{
	mControlPoints.push_back(point);
}

void Interpolate(int shaderProgram, glm::vec3 coord, glm::vec3 coord2) {

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	GLfloat movementSpeed = 5.0f;
	GLfloat a = 1.0f;
	a += movementSpeed * deltaTime;
	glm::vec3 direction;



	for (int i = 0; i < mControlPoints.size(); i++)
	{



		direction = mControlPoints[i] * float((a * deltaTime) / 1000.0f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, coord);
		model = glm::scale(model, coord2);
		model = glm::translate(model, direction);
		setWorldMatrix(shaderProgram, model);
	}

}


void Interpolate2(int shaderProgram, glm::vec3 coord, glm::vec3 coord2) {

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	GLfloat movementSpeed = 5.0f;
	GLfloat a = 1.0f;
	a += movementSpeed * deltaTime;
	glm::vec3 direction;
	//variables for rotating the wheels
	float angle = 90.0f;
	float rotationSpeed = 360.0f;
	float lastFrameTime = glfwGetTime();


	for (int i = 0; i < mControlPoints.size(); i++)
	{



		direction = mControlPoints[i] * float((a * deltaTime) / 1000.0f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, coord);
		model = glm::scale(model, coord2);
		model = glm::translate(model, direction);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 10.0f));
		setWorldMatrix(shaderProgram, model);
	}

}






int main(int argc, char* argv[])
{
	
	mControlPoints.push_back(vec3(0.0, 0.0f, 0.0f));
	mControlPoints.push_back(vec3(-10.0, 0.0f, 0.0f));


	// Initialize GLFW and OpenGL version
	glfwInit();

	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


	// Create Window and rendering context using GLFW
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Comp371Assignment1", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Load Textures
	GLuint grassTextureID = loadTexture("grass.jpg");
	GLuint tireTextureID = loadTexture("tire.jpg");
	GLuint smokeTextureID = loadTexture("smoke.jpg");
	GLuint templeRoofTextureID = loadTexture("../Assets/Models/Temple/Textures/Temple_Roof.jpg");
	GLuint templePillerTextureID = loadTexture("../Assets/Models/Temple/Textures/Temple_Piller.jpg");
	GLuint templeFloorTextureID = loadTexture("../Assets/Models/Temple/Textures/Temple_Floor.jpg");
	GLuint templePillerFloorTextureID = loadTexture("../Assets/Models/Temple/Textures/Temple_Piller_Floor.jpg");
	GLuint tntTextureID = loadTexture("../Assets/Models/Crate/tnt.jpg");
	GLuint goldTextureID = loadTexture("gold.jpg");
	GLuint treeTextureID = loadTexture("../Assets/Models/Tree/tree.jpeg");

	goldTextureContainer = goldTextureID;
	tireTextureContainer = tireTextureID;


	// Compile and link shaders here ...
	int colorShaderProgram = compileAndLinkShaders("vertex.shader", "fragment.shader");
	int textureShaderProgram = compileAndLinkShaders("vertextexture.shader", "fragmenttexture.shader");
	int lightingShaderProgram = compileAndLinkShaders("lighting_vertex_ex.shader", "lighting_fragment_ex.shader");
	int lightingShaderProgram2 = compileAndLinkShaders("lighting_vertex_ex2.shader", "lighting_fragment_ex2.shader");
	int lampShaderProgram = compileAndLinkShaders("lamp_vertex.shader", "lamp_fragment.shader");
	int carLampShaderProgram = compileAndLinkShaders("lamp_vertex.shader", "lamp_fragmentcarlights.shader");
	int shadowShaderProgram = compileAndLinkShaders("shadow_mapping_vertex.shader", "shadow_mapping_fragment.shader");
	int shadowDepthShaderProgram = compileAndLinkShaders("shadow_mapping_depth_vertex.shader", "shadow_mapping_depth_fragment.shader");
	int objectShaderProgram = compileAndLinkShaders("vertexObject.shader","fragmentObject.shader");
	int particleShaderProgram = compileAndLinkShaders("particle_vertex.shader", "particle_fragment.shader");
	int pathLineShaderProgram= compileAndLinkShaders("pathlines_vertex.shader", "pathlines_fragment.shader");

	//shadow mapping code taken from https://learnopengl.com/Getting-started/Camera . creates depth map texture

	// configure depth map FBO
	// -----------------------
	GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	glUseProgram(shadowShaderProgram);

	// Set shadow properties

	glUniform1f(glGetUniformLocation(shadowShaderProgram, "diffuseTexture"), 0);
	glUniform1f(glGetUniformLocation(shadowShaderProgram, "shadowMap"), 1);


	//load objects
	string treePath = "../Assets/Models/Tree/Tree.obj";
	string cratePath = "../Assets/Models/Crate/Wooden Crate.obj";
	string templePath = "../Assets/Models/Temple/Basic Temple.obj";

	int treeVertices;
	GLuint treeVAO = setupModelVBO(treePath, treeVertices);

	int crateVertices;
	GLuint crateVAO = setupModelVBO(cratePath, crateVertices);

	int templeVertices;
	GLuint templeVAO = setupModelVBO(templePath, templeVertices);


	//The following particle code was taken from http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/#particles-lots-of-them-

	static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	static GLubyte* g_particule_color_data = new GLubyte[MaxParticles * 4];

	for (int i = 0; i < MaxParticles; i++) {
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}



	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};
	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	GLuint particles_position_buffer;
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	GLuint particles_color_buffer;
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);


	double lastTime = glfwGetTime();


	//Grid vertices generated through for loops in order to generate a 100x100 grid
	



	vec3 gridVertices[] = {

		//vertical lines

	vec3(-50.0f, -50.0f, 0.0f),
vec3(50.0f, -50.0f, 0.0f),
vec3(-50.0f, -49.0f, 0.0f),
vec3(50.0f, -49.0f, 0.0f),
vec3(-50.0f, -48.0f, 0.0f),
vec3(50.0f, -48.0f, 0.0f),
vec3(-50.0f, -47.0f, 0.0f),
vec3(50.0f, -47.0f, 0.0f),
vec3(-50.0f, -46.0f, 0.0f),
vec3(50.0f, -46.0f, 0.0f),
vec3(-50.0f, -45.0f, 0.0f),
vec3(50.0f, -45.0f, 0.0f),
vec3(-50.0f, -44.0f, 0.0f),
vec3(50.0f, -44.0f, 0.0f),
vec3(-50.0f, -43.0f, 0.0f),
vec3(50.0f, -43.0f, 0.0f),
vec3(-50.0f, -42.0f, 0.0f),
vec3(50.0f, -42.0f, 0.0f),
vec3(-50.0f, -41.0f, 0.0f),
vec3(50.0f, -41.0f, 0.0f),
vec3(-50.0f, -40.0f, 0.0f),
vec3(50.0f, -40.0f, 0.0f),
vec3(-50.0f, -39.0f, 0.0f),
vec3(50.0f, -39.0f, 0.0f),
vec3(-50.0f, -38.0f, 0.0f),
vec3(50.0f, -38.0f, 0.0f),
vec3(-50.0f, -37.0f, 0.0f),
vec3(50.0f, -37.0f, 0.0f),
vec3(-50.0f, -36.0f, 0.0f),
vec3(50.0f, -36.0f, 0.0f),
vec3(-50.0f, -35.0f, 0.0f),
vec3(50.0f, -35.0f, 0.0f),
vec3(-50.0f, -34.0f, 0.0f),
vec3(50.0f, -34.0f, 0.0f),
vec3(-50.0f, -33.0f, 0.0f),
vec3(50.0f, -33.0f, 0.0f),
vec3(-50.0f, -32.0f, 0.0f),
vec3(50.0f, -32.0f, 0.0f),
vec3(-50.0f, -31.0f, 0.0f),
vec3(50.0f, -31.0f, 0.0f),
vec3(-50.0f, -30.0f, 0.0f),
vec3(50.0f, -30.0f, 0.0f),
vec3(-50.0f, -29.0f, 0.0f),
vec3(50.0f, -29.0f, 0.0f),
vec3(-50.0f, -28.0f, 0.0f),
vec3(50.0f, -28.0f, 0.0f),
vec3(-50.0f, -27.0f, 0.0f),
vec3(50.0f, -27.0f, 0.0f),
vec3(-50.0f, -26.0f, 0.0f),
vec3(50.0f, -26.0f, 0.0f),
vec3(-50.0f, -25.0f, 0.0f),
vec3(50.0f, -25.0f, 0.0f),
vec3(-50.0f, -24.0f, 0.0f),
vec3(50.0f, -24.0f, 0.0f),
vec3(-50.0f, -23.0f, 0.0f),
vec3(50.0f, -23.0f, 0.0f),
vec3(-50.0f, -22.0f, 0.0f),
vec3(50.0f, -22.0f, 0.0f),
vec3(-50.0f, -21.0f, 0.0f),
vec3(50.0f, -21.0f, 0.0f),
vec3(-50.0f, -20.0f, 0.0f),
vec3(50.0f, -20.0f, 0.0f),
vec3(-50.0f, -19.0f, 0.0f),
vec3(50.0f, -19.0f, 0.0f),
vec3(-50.0f, -18.0f, 0.0f),
vec3(50.0f, -18.0f, 0.0f),
vec3(-50.0f, -17.0f, 0.0f),
vec3(50.0f, -17.0f, 0.0f),
vec3(-50.0f, -16.0f, 0.0f),
vec3(50.0f, -16.0f, 0.0f),
vec3(-50.0f, -15.0f, 0.0f),
vec3(50.0f, -15.0f, 0.0f),
vec3(-50.0f, -14.0f, 0.0f),
vec3(50.0f, -14.0f, 0.0f),
vec3(-50.0f, -13.0f, 0.0f),
vec3(50.0f, -13.0f, 0.0f),
vec3(-50.0f, -12.0f, 0.0f),
vec3(50.0f, -12.0f, 0.0f),
vec3(-50.0f, -11.0f, 0.0f),
vec3(50.0f, -11.0f, 0.0f),
vec3(-50.0f, -10.0f, 0.0f),
vec3(50.0f, -10.0f, 0.0f),
vec3(-50.0f, -9.0f, 0.0f),
vec3(50.0f, -9.0f, 0.0f),
vec3(-50.0f, -8.0f, 0.0f),
vec3(50.0f, -8.0f, 0.0f),
vec3(-50.0f, -7.0f, 0.0f),
vec3(50.0f, -7.0f, 0.0f),
vec3(-50.0f, -6.0f, 0.0f),
vec3(50.0f, -6.0f, 0.0f),
vec3(-50.0f, -5.0f, 0.0f),
vec3(50.0f, -5.0f, 0.0f),
vec3(-50.0f, -4.0f, 0.0f),
vec3(50.0f, -4.0f, 0.0f),
vec3(-50.0f, -3.0f, 0.0f),
vec3(50.0f, -3.0f, 0.0f),
vec3(-50.0f, -2.0f, 0.0f),
vec3(50.0f, -2.0f, 0.0f),
vec3(-50.0f, -1.0f, 0.0f),
vec3(50.0f, -1.0f, 0.0f),
vec3(-50.0f, 0.0f, 0.0f),
vec3(0.0f, 0.0f, 0.0f),
vec3(0.0f, 0.0f, 0.0f), // x axis
vec3(5.0f, 0.0f, 0.0f), //
vec3(5.0f, 0.0f, 0.0f),
vec3(50.0f, 0.0f, 0.0f),
vec3(-50.0f, 1.0f, 0.0f),
vec3(50.0f, 1.0f, 0.0f),
vec3(-50.0f, 2.0f, 0.0f),
vec3(50.0f, 2.0f, 0.0f),
vec3(-50.0f, 3.0f, 0.0f),
vec3(50.0f, 3.0f, 0.0f),
vec3(-50.0f, 4.0f, 0.0f),
vec3(50.0f, 4.0f, 0.0f),
vec3(-50.0f, 5.0f, 0.0f),
vec3(50.0f, 5.0f, 0.0f),
vec3(-50.0f, 6.0f, 0.0f),
vec3(50.0f, 6.0f, 0.0f),
vec3(-50.0f, 7.0f, 0.0f),
vec3(50.0f, 7.0f, 0.0f),
vec3(-50.0f, 8.0f, 0.0f),
vec3(50.0f, 8.0f, 0.0f),
vec3(-50.0f, 9.0f, 0.0f),
vec3(50.0f, 9.0f, 0.0f),
vec3(-50.0f, 10.0f, 0.0f),
vec3(50.0f, 10.0f, 0.0f),
vec3(-50.0f, 11.0f, 0.0f),
vec3(50.0f, 11.0f, 0.0f),
vec3(-50.0f, 12.0f, 0.0f),
vec3(50.0f, 12.0f, 0.0f),
vec3(-50.0f, 13.0f, 0.0f),
vec3(50.0f, 13.0f, 0.0f),
vec3(-50.0f, 14.0f, 0.0f),
vec3(50.0f, 14.0f, 0.0f),
vec3(-50.0f, 15.0f, 0.0f),
vec3(50.0f, 15.0f, 0.0f),
vec3(-50.0f, 16.0f, 0.0f),
vec3(50.0f, 16.0f, 0.0f),
vec3(-50.0f, 17.0f, 0.0f),
vec3(50.0f, 17.0f, 0.0f),
vec3(-50.0f, 18.0f, 0.0f),
vec3(50.0f, 18.0f, 0.0f),
vec3(-50.0f, 19.0f, 0.0f),
vec3(50.0f, 19.0f, 0.0f),
vec3(-50.0f, 20.0f, 0.0f),
vec3(50.0f, 20.0f, 0.0f),
vec3(-50.0f, 21.0f, 0.0f),
vec3(50.0f, 21.0f, 0.0f),
vec3(-50.0f, 22.0f, 0.0f),
vec3(50.0f, 22.0f, 0.0f),
vec3(-50.0f, 23.0f, 0.0f),
vec3(50.0f, 23.0f, 0.0f),
vec3(-50.0f, 24.0f, 0.0f),
vec3(50.0f, 24.0f, 0.0f),
vec3(-50.0f, 25.0f, 0.0f),
vec3(50.0f, 25.0f, 0.0f),
vec3(-50.0f, 26.0f, 0.0f),
vec3(50.0f, 26.0f, 0.0f),
vec3(-50.0f, 27.0f, 0.0f),
vec3(50.0f, 27.0f, 0.0f),
vec3(-50.0f, 28.0f, 0.0f),
vec3(50.0f, 28.0f, 0.0f),
vec3(-50.0f, 29.0f, 0.0f),
vec3(50.0f, 29.0f, 0.0f),
vec3(-50.0f, 30.0f, 0.0f),
vec3(50.0f, 30.0f, 0.0f),
vec3(-50.0f, 31.0f, 0.0f),
vec3(50.0f, 31.0f, 0.0f),
vec3(-50.0f, 32.0f, 0.0f),
vec3(50.0f, 32.0f, 0.0f),
vec3(-50.0f, 33.0f, 0.0f),
vec3(50.0f, 33.0f, 0.0f),
vec3(-50.0f, 34.0f, 0.0f),
vec3(50.0f, 34.0f, 0.0f),
vec3(-50.0f, 35.0f, 0.0f),
vec3(50.0f, 35.0f, 0.0f),
vec3(-50.0f, 36.0f, 0.0f),
vec3(50.0f, 36.0f, 0.0f),
vec3(-50.0f, 37.0f, 0.0f),
vec3(50.0f, 37.0f, 0.0f),
vec3(-50.0f, 38.0f, 0.0f),
vec3(50.0f, 38.0f, 0.0f),
vec3(-50.0f, 39.0f, 0.0f),
vec3(50.0f, 39.0f, 0.0f),
vec3(-50.0f, 40.0f, 0.0f),
vec3(50.0f, 40.0f, 0.0f),
vec3(-50.0f, 41.0f, 0.0f),
vec3(50.0f, 41.0f, 0.0f),
vec3(-50.0f, 42.0f, 0.0f),
vec3(50.0f, 42.0f, 0.0f),
vec3(-50.0f, 43.0f, 0.0f),
vec3(50.0f, 43.0f, 0.0f),
vec3(-50.0f, 44.0f, 0.0f),
vec3(50.0f, 44.0f, 0.0f),
vec3(-50.0f, 45.0f, 0.0f),
vec3(50.0f, 45.0f, 0.0f),
vec3(-50.0f, 46.0f, 0.0f),
vec3(50.0f, 46.0f, 0.0f),
vec3(-50.0f, 47.0f, 0.0f),
vec3(50.0f, 47.0f, 0.0f),
vec3(-50.0f, 48.0f, 0.0f),
vec3(50.0f, 48.0f, 0.0f),
vec3(-50.0f, 49.0f, 0.0f),
vec3(50.0f, 49.0f, 0.0f),
vec3(-50.0f, 50.0f, 0.0f),
vec3(50.0f, 50.0f, 0.0f),
vec3(-50.0f, -50.0f, 0.0f),
vec3(-50.0f, 50.0f, 0.0f),
vec3(-49.0f, -50.0f, 0.0f),
vec3(-49.0f, 50.0f, 0.0f),
vec3(-48.0f, -50.0f, 0.0f),
vec3(-48.0f, 50.0f, 0.0f),
vec3(-47.0f, -50.0f, 0.0f),
vec3(-47.0f, 50.0f, 0.0f),
vec3(-46.0f, -50.0f, 0.0f),
vec3(-46.0f, 50.0f, 0.0f),
vec3(-45.0f, -50.0f, 0.0f),
vec3(-45.0f, 50.0f, 0.0f),
vec3(-44.0f, -50.0f, 0.0f),
vec3(-44.0f, 50.0f, 0.0f),
vec3(-43.0f, -50.0f, 0.0f),
vec3(-43.0f, 50.0f, 0.0f),
vec3(-42.0f, -50.0f, 0.0f),
vec3(-42.0f, 50.0f, 0.0f),
vec3(-41.0f, -50.0f, 0.0f),
vec3(-41.0f, 50.0f, 0.0f),
vec3(-40.0f, -50.0f, 0.0f),
vec3(-40.0f, 50.0f, 0.0f),
vec3(-39.0f, -50.0f, 0.0f),
vec3(-39.0f, 50.0f, 0.0f),
vec3(-38.0f, -50.0f, 0.0f),
vec3(-38.0f, 50.0f, 0.0f),
vec3(-37.0f, -50.0f, 0.0f),
vec3(-37.0f, 50.0f, 0.0f),
vec3(-36.0f, -50.0f, 0.0f),
vec3(-36.0f, 50.0f, 0.0f),
vec3(-35.0f, -50.0f, 0.0f),
vec3(-35.0f, 50.0f, 0.0f),
vec3(-34.0f, -50.0f, 0.0f),
vec3(-34.0f, 50.0f, 0.0f),
vec3(-33.0f, -50.0f, 0.0f),
vec3(-33.0f, 50.0f, 0.0f),
vec3(-32.0f, -50.0f, 0.0f),
vec3(-32.0f, 50.0f, 0.0f),
vec3(-31.0f, -50.0f, 0.0f),
vec3(-31.0f, 50.0f, 0.0f),
vec3(-30.0f, -50.0f, 0.0f),
vec3(-30.0f, 50.0f, 0.0f),
vec3(-29.0f, -50.0f, 0.0f),
vec3(-29.0f, 50.0f, 0.0f),
vec3(-28.0f, -50.0f, 0.0f),
vec3(-28.0f, 50.0f, 0.0f),
vec3(-27.0f, -50.0f, 0.0f),
vec3(-27.0f, 50.0f, 0.0f),
vec3(-26.0f, -50.0f, 0.0f),
vec3(-26.0f, 50.0f, 0.0f),
vec3(-25.0f, -50.0f, 0.0f),
vec3(-25.0f, 50.0f, 0.0f),
vec3(-24.0f, -50.0f, 0.0f),
vec3(-24.0f, 50.0f, 0.0f),
vec3(-23.0f, -50.0f, 0.0f),
vec3(-23.0f, 50.0f, 0.0f),
vec3(-22.0f, -50.0f, 0.0f),
vec3(-22.0f, 50.0f, 0.0f),
vec3(-21.0f, -50.0f, 0.0f),
vec3(-21.0f, 50.0f, 0.0f),
vec3(-20.0f, -50.0f, 0.0f),
vec3(-20.0f, 50.0f, 0.0f),
vec3(-19.0f, -50.0f, 0.0f),
vec3(-19.0f, 50.0f, 0.0f),
vec3(-18.0f, -50.0f, 0.0f),
vec3(-18.0f, 50.0f, 0.0f),
vec3(-17.0f, -50.0f, 0.0f),
vec3(-17.0f, 50.0f, 0.0f),
vec3(-16.0f, -50.0f, 0.0f),
vec3(-16.0f, 50.0f, 0.0f),
vec3(-15.0f, -50.0f, 0.0f),
vec3(-15.0f, 50.0f, 0.0f),
vec3(-14.0f, -50.0f, 0.0f),
vec3(-14.0f, 50.0f, 0.0f),
vec3(-13.0f, -50.0f, 0.0f),
vec3(-13.0f, 50.0f, 0.0f),
vec3(-12.0f, -50.0f, 0.0f),
vec3(-12.0f, 50.0f, 0.0f),
vec3(-11.0f, -50.0f, 0.0f),
vec3(-11.0f, 50.0f, 0.0f),
vec3(-10.0f, -50.0f, 0.0f),
vec3(-10.0f, 50.0f, 0.0f),
vec3(-9.0f, -50.0f, 0.0f),
vec3(-9.0f, 50.0f, 0.0f),
vec3(-8.0f, -50.0f, 0.0f),
vec3(-8.0f, 50.0f, 0.0f),
vec3(-7.0f, -50.0f, 0.0f),
vec3(-7.0f, 50.0f, 0.0f),
vec3(-6.0f, -50.0f, 0.0f),
vec3(-6.0f, 50.0f, 0.0f),
vec3(-5.0f, -50.0f, 0.0f),
vec3(-5.0f, 50.0f, 0.0f),
vec3(-4.0f, -50.0f, 0.0f),
vec3(-4.0f, 50.0f, 0.0f),
vec3(-3.0f, -50.0f, 0.0f),
vec3(-3.0f, 50.0f, 0.0f),
vec3(-2.0f, -50.0f, 0.0f),
vec3(-2.0f, 50.0f, 0.0f),
vec3(-1.0f, -50.0f, 0.0f),
vec3(-1.0f, 50.0f, 0.0f),
vec3(0.0f, -50.0f, 0.0f),
vec3(0.0f, 0.0f, 0.0f),
vec3(0.0f, 0.0f, 0.0f), //
vec3(0.0f, 5.0f, 0.0f), //  y axis
vec3(0.0f, 5.0f, 0.0f),
vec3(0.0f, 50.0f, 0.0f),
vec3(1.0f, -50.0f, 0.0f),
vec3(1.0f, 50.0f, 0.0f),
vec3(2.0f, -50.0f, 0.0f),
vec3(2.0f, 50.0f, 0.0f),
vec3(3.0f, -50.0f, 0.0f),
vec3(3.0f, 50.0f, 0.0f),
vec3(4.0f, -50.0f, 0.0f),
vec3(4.0f, 50.0f, 0.0f),
vec3(5.0f, -50.0f, 0.0f),
vec3(5.0f, 50.0f, 0.0f),
vec3(6.0f, -50.0f, 0.0f),
vec3(6.0f, 50.0f, 0.0f),
vec3(7.0f, -50.0f, 0.0f),
vec3(7.0f, 50.0f, 0.0f),
vec3(8.0f, -50.0f, 0.0f),
vec3(8.0f, 50.0f, 0.0f),
vec3(9.0f, -50.0f, 0.0f),
vec3(9.0f, 50.0f, 0.0f),
vec3(10.0f, -50.0f, 0.0f),
vec3(10.0f, 50.0f, 0.0f),
vec3(11.0f, -50.0f, 0.0f),
vec3(11.0f, 50.0f, 0.0f),
vec3(12.0f, -50.0f, 0.0f),
vec3(12.0f, 50.0f, 0.0f),
vec3(13.0f, -50.0f, 0.0f),
vec3(13.0f, 50.0f, 0.0f),
vec3(14.0f, -50.0f, 0.0f),
vec3(14.0f, 50.0f, 0.0f),
vec3(15.0f, -50.0f, 0.0f),
vec3(15.0f, 50.0f, 0.0f),
vec3(16.0f, -50.0f, 0.0f),
vec3(16.0f, 50.0f, 0.0f),
vec3(17.0f, -50.0f, 0.0f),
vec3(17.0f, 50.0f, 0.0f),
vec3(18.0f, -50.0f, 0.0f),
vec3(18.0f, 50.0f, 0.0f),
vec3(19.0f, -50.0f, 0.0f),
vec3(19.0f, 50.0f, 0.0f),
vec3(20.0f, -50.0f, 0.0f),
vec3(20.0f, 50.0f, 0.0f),
vec3(21.0f, -50.0f, 0.0f),
vec3(21.0f, 50.0f, 0.0f),
vec3(22.0f, -50.0f, 0.0f),
vec3(22.0f, 50.0f, 0.0f),
vec3(23.0f, -50.0f, 0.0f),
vec3(23.0f, 50.0f, 0.0f),
vec3(24.0f, -50.0f, 0.0f),
vec3(24.0f, 50.0f, 0.0f),
vec3(25.0f, -50.0f, 0.0f),
vec3(25.0f, 50.0f, 0.0f),
vec3(26.0f, -50.0f, 0.0f),
vec3(26.0f, 50.0f, 0.0f),
vec3(27.0f, -50.0f, 0.0f),
vec3(27.0f, 50.0f, 0.0f),
vec3(28.0f, -50.0f, 0.0f),
vec3(28.0f, 50.0f, 0.0f),
vec3(29.0f, -50.0f, 0.0f),
vec3(29.0f, 50.0f, 0.0f),
vec3(30.0f, -50.0f, 0.0f),
vec3(30.0f, 50.0f, 0.0f),
vec3(31.0f, -50.0f, 0.0f),
vec3(31.0f, 50.0f, 0.0f),
vec3(32.0f, -50.0f, 0.0f),
vec3(32.0f, 50.0f, 0.0f),
vec3(33.0f, -50.0f, 0.0f),
vec3(33.0f, 50.0f, 0.0f),
vec3(34.0f, -50.0f, 0.0f),
vec3(34.0f, 50.0f, 0.0f),
vec3(35.0f, -50.0f, 0.0f),
vec3(35.0f, 50.0f, 0.0f),
vec3(36.0f, -50.0f, 0.0f),
vec3(36.0f, 50.0f, 0.0f),
vec3(37.0f, -50.0f, 0.0f),
vec3(37.0f, 50.0f, 0.0f),
vec3(38.0f, -50.0f, 0.0f),
vec3(38.0f, 50.0f, 0.0f),
vec3(39.0f, -50.0f, 0.0f),
vec3(39.0f, 50.0f, 0.0f),
vec3(40.0f, -50.0f, 0.0f),
vec3(40.0f, 50.0f, 0.0f),
vec3(41.0f, -50.0f, 0.0f),
vec3(41.0f, 50.0f, 0.0f),
vec3(42.0f, -50.0f, 0.0f),
vec3(42.0f, 50.0f, 0.0f),
vec3(43.0f, -50.0f, 0.0f),
vec3(43.0f, 50.0f, 0.0f),
vec3(44.0f, -50.0f, 0.0f),
vec3(44.0f, 50.0f, 0.0f),
vec3(45.0f, -50.0f, 0.0f),
vec3(45.0f, 50.0f, 0.0f),
vec3(46.0f, -50.0f, 0.0f),
vec3(46.0f, 50.0f, 0.0f),
vec3(47.0f, -50.0f, 0.0f),
vec3(47.0f, 50.0f, 0.0f),
vec3(48.0f, -50.0f, 0.0f),
vec3(48.0f, 50.0f, 0.0f),
vec3(49.0f, -50.0f, 0.0f),
vec3(49.0f, 50.0f, 0.0f),
vec3(50.0f, -50.0f, 0.0f),
vec3(50.0f, 50.0f, 0.0f),
vec3(0.0f, 0.0f, 0.0f), // z axis
vec3(0.0f, 0.0f, 5.0f) // z axis

	};

	//color vertices

	vec3 gridcolors[] = {
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(0.0f,1.0f,0.0f), //x axis
vec3(0.0f,1.0f,0.0f), // x axis
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(0.0f,1.0f,1.0f), //y axis
vec3(0.0f,1.0f,1.0f), // y axis
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f,0.0f,0.0f),
vec3(1.0f, 0.0f, 0.0f),
vec3(1.0f, 0.0f, 0.0f),
vec3(1.0f, 0.0f, 0.0f),
vec3(1.0f, 0.0f, 0.0f),
vec3(1.0f, 0.0f, 0.0f),
vec3(1.0f, 0.0f, 0.0f),
vec3(1.0f, 0.0f, 1.0f), // z axis
vec3(1.0f, 0.0f, 1.0f), // z axis

	};


	// texture vertices

	vec2 gridTextures[] = {
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(0.0f,0.0f),
vec2(0.0f,1.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),
vec2(1.0f,1.0f),
vec2(0.0f,0.0f),

	};


	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);



	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec3),
		(void*)0
	);




	GLuint vertexBufferObject2;
	glGenBuffers(1, &vertexBufferObject2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gridcolors), gridcolors, GL_STATIC_DRAW);


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec3),
		(void*)0
	);


	GLuint vertexBufferObject3;
	glGenBuffers(1, &vertexBufferObject3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gridTextures), gridTextures, GL_STATIC_DRAW);


	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec2),
		(void*)0
	);



	//cube vertices 

	TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
		TexturedColoredVertex(vec3(-1.0f,-1.0f,-1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)), //left - red
		TexturedColoredVertex(vec3(-1.0f,-1.0f, 1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
		TexturedColoredVertex(vec3(-1.0f, 1.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),

		TexturedColoredVertex(vec3(-1.0f,-1.0f,-1.0f),  vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(-1.0f, 1.0f, 1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(-1.0f, 1.0f,-1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(1.0f, 1.0f,-1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // far - blue
		TexturedColoredVertex(vec3(-1.0f,-1.0f,-1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(-1.0f, 1.0f,-1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

		TexturedColoredVertex(vec3(1.0f, 1.0f,-1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(1.0f,-1.0f,-1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
		TexturedColoredVertex(vec3(-1.0f,-1.0f,-1.0f),  vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

		TexturedColoredVertex(vec3(1.0f,-1.0f, 1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
		TexturedColoredVertex(vec3(-1.0f,-1.0f,-1.0f),    vec3(0.0f, 1.0f, 1.0f),vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(1.0f,-1.0f,-1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(1.0f,-1.0f, 1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(-1.0f,-1.0f, 1.0f),  vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
		TexturedColoredVertex(vec3(-1.0f,-1.0f,-1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

		TexturedColoredVertex(vec3(-1.0f, 1.0f, 1.0f),  vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)), // near - green
		TexturedColoredVertex(vec3(-1.0f,-1.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(1.0f,-1.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(1.0f, 1.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(-1.0f, 1.0f, 1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
		TexturedColoredVertex(vec3(1.0f,-1.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(1.0f, 1.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // right - purple
		TexturedColoredVertex(vec3(1.0f,-1.0f,-1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(1.0f, 1.0f,-1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(1.0f,-1.0f,-1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(1.0f, 1.0f, 1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(1.0f,-1.0f, 1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

		TexturedColoredVertex(vec3(1.0f, 1.0f, 1.0f),  vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // top - yellow
		TexturedColoredVertex(vec3(1.0f, 1.0f,-1.0f),    vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
		TexturedColoredVertex(vec3(-1.0f, 1.0f,-1.0f),   vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

		TexturedColoredVertex(vec3(1.0f, 1.0f, 1.0f),  vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(-1.0f, 1.0f,-1.0f),  vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(-1.0f, 1.0f, 1.0f),  vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f))

	};

	// Create a vertex array
	GLuint vertexArrayObject1;
	glGenVertexArrays(1, &vertexArrayObject1);
	glBindVertexArray(vertexArrayObject1);


	GLuint vertexBufferObject1;
	glGenBuffers(1, &vertexBufferObject1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)0
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)sizeof(vec3)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)(2 * sizeof(vec3))
	);
	glEnableVertexAttribArray(2);

	GLuint
		lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (void*)0);
	glEnableVertexAttribArray(0);


	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//hide mousr cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//initialize cursor at the center
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	//enable mouse controls
	glfwSetCursorPosCallback(window, mouse_callback);
	 glfwSetScrollCallback(window, scroll_callback);



	//variables for rotating the wheels
	float angle = 90.0f;
	float rotationSpeed = 360.0f;
	float lastFrameTime = glfwGetTime();

	//projection matrix coordinates
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);

	setProjectionMatrix(textureShaderProgram, projectionMatrix);
	setProjectionMatrix(lightingShaderProgram, projectionMatrix);
	setProjectionMatrix(shadowShaderProgram, projectionMatrix);
	setProjectionMatrix(lampShaderProgram, projectionMatrix);
	setProjectionMatrix(objectShaderProgram, projectionMatrix);
	setProjectionMatrix(carLampShaderProgram, projectionMatrix);
	setProjectionMatrix(lightingShaderProgram2, projectionMatrix);

	//view matrix coordinates 
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(cameraPosition, cameraFront, cameraUp);


	//used for playing music

	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		return 0; // error starting up the engine


		// play some sound stream, looped

	engine->play2D("WHEELS ON THE BUS (TRAP REMIX).mp3", true);
	
	bool  cameraFirstPerson = true;// press c to toggle this variable




	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{

		//declare method to process keyboard input
		keyboardInput(window);

		//calculating the rotation of the wheels
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		angle = (angle + rotationSpeed * dt);

		//data for lighting
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;

		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		//activate depth map texture
		setlightSpaceMatrix(shadowDepthShaderProgram, lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTextureID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Each frame, reset color of each pixel to glClearColor

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//set light uniforms in shader
		GLint lightPosLoc = glGetUniformLocation(lightingShaderProgram, "light.position");
		GLint viewPosLoc = glGetUniformLocation(lightingShaderProgram, "viewPos");


		GLint lightPosLoc2 = glGetUniformLocation(lightingShaderProgram2, "light.position");
		GLint viewPosLoc2 = glGetUniformLocation(lightingShaderProgram2, "viewPos");

		glUseProgram(lightingShaderProgram);

		// Set lights properties
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(lightingShaderProgram, "light.ambient"), 100.2f, 100.2f, 100.2f);
		glUniform3f(glGetUniformLocation(lightingShaderProgram, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShaderProgram, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShaderProgram, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShaderProgram, "light.linear"), 0.9);
		glUniform1f(glGetUniformLocation(lightingShaderProgram, "light.quadratic"), 0.032);
		glUniform1f(glGetUniformLocation(lightingShaderProgram, "material.shininess"), 32.0f);
		glUniform3f(viewPosLoc, 1.5f, 2.0f, 0.0f);


		glUseProgram(lightingShaderProgram2);

		// Set lights properties
		glUniform3f(lightPosLoc2, headlight1.x, headlight1.y, headlight1.z);
		glUniform3f(glGetUniformLocation(lightingShaderProgram2, "light.ambient"), headlights.x, headlights.y, headlights.z);
		glUniform3f(glGetUniformLocation(lightingShaderProgram2, "light.diffuse"), 50.5f, 50.5f, 50.5f);
		glUniform3f(glGetUniformLocation(lightingShaderProgram2, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShaderProgram2, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShaderProgram2, "light.linear"), 0.9);
		glUniform1f(glGetUniformLocation(lightingShaderProgram2, "light.quadratic"), 0.032);
		glUniform1f(glGetUniformLocation(lightingShaderProgram2, "material.shininess"), 32.0f);
		glUniform3f(viewPosLoc2, 1.5f, 2.0f, 0.0f);

	


		glUseProgram(shadowShaderProgram);

		setlightSpaceMatrix(shadowShaderProgram, lightSpaceMatrix);
		glUniform3f(glGetUniformLocation(shadowShaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shadowShaderProgram, "viewPos"), 1.5f, 2.0f, 0.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);



		//set texture uniforms
		glUniform1i(glGetUniformLocation(textureShaderProgram, "textureSampler"),0);
		glUniform1i(glGetUniformLocation(objectShaderProgram, "texture_diffuse1"), 1);

	
		glActiveTexture(GL_TEXTURE0);



		//bind light point cube VAO
		glBindVertexArray(lightVAO);

		glm::mat4 model14 = glm::mat4(1.0f);
		model14 = glm::translate(model14, lightPos);
		model14 = glm::scale(model14, glm::vec3(5.2f));

		
		setWorldMatrix(lampShaderProgram, model14);

		//draw light point cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//unbind VAO
		glBindVertexArray(0);




		//draw headlight
		glBindVertexArray(lightVAO);

		glm::mat4 model20 = glm::mat4(1.0f);
		model20 = glm::translate(model20, headlight1);
		model20 = glm::scale(model20, glm::vec3(0.4f));

		// Interpolate(lampShaderProgram, headlight1, glm::vec3(0.4f));
		setWorldMatrix(lampShaderProgram, model20);
	

		//draw light point cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//unbind VAO
		glBindVertexArray(0);


		//draw headlight
		glBindVertexArray(lightVAO);

		glm::mat4 model21 = glm::mat4(1.0f);
		model21 = glm::translate(model21, headlight2);
		model21 = glm::scale(model21, glm::vec3(0.4f));

		 //Interpolate(lampShaderProgram, headlight2, glm::vec3(0.4f));
		setWorldMatrix(lampShaderProgram, model21);

		

		//draw light point cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//unbind VAO
		glBindVertexArray(0);





		//draw tailight1
		glBindVertexArray(lightVAO);

		glm::mat4 model22 = glm::mat4(1.0f);
		model22 = glm::translate(model22, tailight1);
		model22 = glm::scale(model22, glm::vec3(0.4f));

		 //Interpolate(carLampShaderProgram, tailight1, glm::vec3(0.4f));
		setWorldMatrix(carLampShaderProgram, model22);

		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//unbind VAO
		glBindVertexArray(0);


		//draw tailight2
		glBindVertexArray(lightVAO);

		glm::mat4 model23 = glm::mat4(1.0f);
		model23 = glm::translate(model23, tailight2);
		model23 = glm::scale(model23, glm::vec3(0.4f));

		// Interpolate(carLampShaderProgram, tailight2, glm::vec3(0.4f));
		setWorldMatrix(carLampShaderProgram, model23);

		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//unbind VAO
		glBindVertexArray(0);



		//bind grid VAO
		glBindVertexArray(vertexArrayObject);



		//Draw grid
		glBindTexture(GL_TEXTURE_2D, grassTextureID);
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		setWorldMatrix(lightingShaderProgram, rotationMatrix);
		setWorldMatrix(shadowShaderProgram, rotationMatrix);
		setWorldMatrix(lightingShaderProgram2, rotationMatrix);
	
		
	
		
	

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 400);

		// Unbind VAO
		glBindVertexArray(0);



		//bind  VAO
		glBindVertexArray(vertexArrayObject1);

		glBindTexture(GL_TEXTURE_2D, goldTextureContainer);

		//Interpolate(lightingShaderProgram, body, body1);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, body);
		model = glm::scale(model, body1);
		setWorldMatrix(shadowShaderProgram, model);
		setWorldMatrix(lightingShaderProgram, model);
	
		// draw body of car

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// Unbind VAO
		glBindVertexArray(0);



		glBindVertexArray(vertexArrayObject1);


		glBindTexture(GL_TEXTURE_2D, tireTextureContainer);
		
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, left_back_wheel);
		model1 = glm::scale(model1, left_back_wheel1);
		model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(0.0f, 1.0f, 10.0f));
		setWorldMatrix(shadowShaderProgram,model1);
		setWorldMatrix(lightingShaderProgram, model1);

		 //Interpolate2(lightingShaderProgram, left_back_wheel, left_back_wheel1);

		//draw left rear wheel
		glDrawArrays(GL_TRIANGLES, 0, 36);





		// Unbind VAO
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);



		glBindVertexArray(vertexArrayObject1);



		glBindTexture(GL_TEXTURE_2D, tireTextureContainer);
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, left_front_wheel);
		model2 = glm::scale(model2, left_front_wheel1);
		model2 = glm::rotate(model2, glm::radians(angle), glm::vec3(0.0f, 1.0f, 10.0f));
		setWorldMatrix(shadowShaderProgram, model2);
		setWorldMatrix(lightingShaderProgram, model2);

		 //Interpolate2(lightingShaderProgram, left_front_wheel, left_front_wheel1);

		//draw left front wheel
		glDrawArrays(GL_TRIANGLES, 0, 36);





		// Unbind VAO
		glBindVertexArray(0);

		//draw front right wheel

		glBindVertexArray(vertexArrayObject1);

		glBindTexture(GL_TEXTURE_2D, tireTextureContainer);
		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::translate(model3, right_front_wheel);
		model3 = glm::scale(model3, right_front_wheel1);
		model3 = glm::rotate(model3, glm::radians(angle), glm::vec3(0.0f, 1.0f, 10.0f));
		setWorldMatrix(shadowShaderProgram, model3);
		setWorldMatrix(lightingShaderProgram, model3);


		 //Interpolate2(lightingShaderProgram, right_front_wheel, right_front_wheel1);

		glDrawArrays(GL_TRIANGLES, 0, 36);





		// Unbind VAO
		glBindVertexArray(0);


		glBindVertexArray(vertexArrayObject1);



		glBindTexture(GL_TEXTURE_2D, tireTextureContainer);
		glm::mat4 model4 = glm::mat4(1.0f);
		model4 = glm::translate(model4, right_rear_wheel);
		model4 = glm::scale(model4, right_rear_wheel1);
		model4 = glm::rotate(model4, glm::radians(angle), glm::vec3(0.0f, 1.0f, 10.0f));
		setWorldMatrix(shadowShaderProgram, model4);
		setWorldMatrix(lightingShaderProgram, model4);
	

		 //Interpolate2(lightingShaderProgram, right_rear_wheel, right_rear_wheel1);

		//draw right rear wheel
		glDrawArrays(GL_TRIANGLES, 0, 36);





		// Unbind VAO
		glBindVertexArray(0);



		glBindVertexArray(vertexArrayObject1);




		glBindTexture(GL_TEXTURE_2D, goldTextureContainer);
		glm::mat4 model5 = glm::mat4(1.0f);
		model5 = glm::translate(model5, roof);
		model5 = glm::scale(model5, roof1);
		setWorldMatrix(shadowShaderProgram, model5);
		setWorldMatrix(lightingShaderProgram, model5);
		// Interpolate(lightingShaderProgram, roof, roof1);

		//draw roof
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// Unbind VAO
		glBindVertexArray(0);


		glBindVertexArray(vertexArrayObject1);



		glBindTexture(GL_TEXTURE_2D, goldTextureContainer);
		glm::mat4 model6 = glm::mat4(1.0f);
		model6 = glm::translate(model6, trunk);
		model6 = glm::scale(model6, trunk1);
		setWorldMatrix(shadowShaderProgram, model6);
	setWorldMatrix(lightingShaderProgram, model6);
		// Interpolate(lightingShaderProgram, trunk, trunk1);

		//draw trunk
		glDrawArrays(GL_TRIANGLES, 0, 36);





		// Unbind VAO
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);



		glBindVertexArray(vertexArrayObject1);



		glBindTexture(GL_TEXTURE_2D, goldTextureContainer);
		glm::mat4 model7 = glm::mat4(1.0f);
		model7 = glm::translate(model7, bonnet);
		model7 = glm::scale(model7, bonnet1);
		setWorldMatrix(shadowShaderProgram, model7);
		setWorldMatrix(lightingShaderProgram, model7);

		 //Interpolate(lightingShaderProgram, bonnet, bonnet1);

		//draw bonnet
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);



		
		//draw tree obj

		glBindVertexArray(treeVAO);
		glBindTexture(GL_TEXTURE_2D, treeTextureID);
		glm::mat4 model8= glm::mat4(1.0f);
		model8 = glm::translate(model8, glm::vec3(-10.0f, 0.0f, -20.0f));
		model8 = glm::scale(model8, glm::vec3(8.0f));
		
		setWorldMatrix(lightingShaderProgram, model8);
		glDrawArrays(GL_TRIANGLES, 0, treeVertices);

		glBindVertexArray(0);


		glBindVertexArray(crateVAO);
		glm::mat4 model9 = glm::mat4(1.0f);
		model9 = glm::translate(model9, glm::vec3(30.0f, 0.0f, -40.0f));
		model9 = glm::scale(model9, glm::vec3(2.0f));
		glBindTexture(GL_TEXTURE_2D, tntTextureID);
	
		setWorldMatrix(lightingShaderProgram, model9);
		
		glDrawArrays(GL_TRIANGLES, 0, crateVertices);

		glBindVertexArray(0);


		glBindVertexArray(templeVAO);
		glm::mat4 model10 = glm::mat4(1.0f);
		glBindTexture(GL_TEXTURE_2D, templeRoofTextureID);
		glBindTexture(GL_TEXTURE_2D, templePillerTextureID);
		glBindTexture(GL_TEXTURE_2D, templeFloorTextureID);
		glBindTexture(GL_TEXTURE_2D, templePillerFloorTextureID);
		model10 = glm::translate(model10, glm::vec3(30.0f, 0.0f, 0.0f));
		model10 = glm::scale(model10, glm::vec3(0.2f));
		setWorldMatrix(lightingShaderProgram, model10);
		glDrawArrays(GL_TRIANGLES, 0, templeVertices);

		glBindVertexArray(0);


		

	

		//The following particle code was taken from http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/#particles-lots-of-them-

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		int newparticles = (int)(delta * 10000.0);
		if (newparticles > (int)(0.016f * 10000.0))
			newparticles = (int)(0.016f * 10000.0);

		for (int i = 0; i < newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = 2.0f; // This particle will live 5 seconds.
			ParticlesContainer[particleIndex].pos = particlePosition;

			float spread = 1.5f;
			glm::vec3 maindir = glm::vec3(10.0f, 5.0f, 5.0f);
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f
			);

			ParticlesContainer[particleIndex].speed = maindir + randomdir * spread;


			// Very bad way to generate a random color
			ParticlesContainer[particleIndex].r = rand() % 256;
			ParticlesContainer[particleIndex].g = rand() % 256;
			ParticlesContainer[particleIndex].g = rand() % 256;
			ParticlesContainer[particleIndex].b = rand() % 256;
			ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

			ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

		}

		int ParticlesCount = 0;
		for (int i = 0; i < MaxParticles; i++) {

			Particle& p = ParticlesContainer[i]; // shortcut

			if (p.life > 0.0f) {

				// Decrease life
				p.life -= delta;
				if (p.life > 0.0f) {

					// Simulate simple physics : gravity only, no collisions
					p.speed += glm::vec3(0.0f, 9.81f, 0.0f) * (float)delta * 0.5f;
					p.pos += p.speed * (float)delta;
					p.cameradistance = glm::length(p.pos - cameraPosition);
					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

					// Fill the GPU buffer
					g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
					g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
					g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

					g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

					g_particule_color_data[4 * ParticlesCount + 0] = p.r;
					g_particule_color_data[4 * ParticlesCount + 1] = p.g;
					g_particule_color_data[4 * ParticlesCount + 2] = p.b;
					g_particule_color_data[4 * ParticlesCount + 3] = p.a;

				}
				else {
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}

				ParticlesCount++;

			}
		}

		SortParticles();



		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLuint TextureID = glGetUniformLocation(particleShaderProgram, "myTextureSampler");
		GLuint CameraRight_worldspace_ID = glGetUniformLocation(particleShaderProgram, "CameraRight_worldspace");
		GLuint CameraUp_worldspace_ID = glGetUniformLocation(particleShaderProgram, "CameraUp_worldspace");
		GLuint ViewProjMatrixID = glGetUniformLocation(particleShaderProgram, "VP");

		glm::mat4 ViewProjectionMatrix = projectionMatrix * viewMatrix;


		// Use our shader
		glUseProgram(particleShaderProgram);
	

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, smokeTextureID);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);


		glUniform3f(CameraRight_worldspace_ID, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
		glUniform3f(CameraUp_worldspace_ID, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

		glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
		

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
		);



		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);



		glfwSwapBuffers(window);
		glfwPollEvents();



		//used for toggling car textures on and off
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {

			goldTextureContainer = 0;
			tireTextureContainer = 0;
			glBindTexture(GL_TEXTURE_2D, goldTextureContainer);
		}

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {

			tireTextureContainer = tireTextureID;
			goldTextureContainer = goldTextureID;
			glBindTexture(GL_TEXTURE_2D, goldTextureContainer);
		}


		// First and third person camera positions taken from lab03, comp 371 tutorial 3

		
		float cameraHorizontalAngle = 90.0f;
		float cameraVerticalAngle = 0.0f;

		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);


		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) 
		{
			cameraFirstPerson = true;
			
		}

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) 
		{
			cameraFirstPerson = false;
		}






		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			glfwSetWindowShouldClose(window, true);
		}


        
        if (cameraFirstPerson)
        {
            viewMatrix = lookAt(cameraPosition,cameraFront, cameraUp );
        }
        if(!cameraFirstPerson)
        {
        
            float radius = 5.0f;
            vec3 position = cameraPosition - vec3(radius * cosf(phi)*cosf(theta),
                                                  radius * sinf(phi),
                                                  -radius * cosf(phi)*sinf(theta));

			vec3 position2 = vec3(1.5f, 7.0f, 0.0f);
;
            viewMatrix = lookAt(position2,position,cameraUp);
        }

		setViewMatrix(textureShaderProgram, viewMatrix);
		setViewMatrix(lightingShaderProgram, viewMatrix);
		setViewMatrix(shadowShaderProgram, viewMatrix);
		setViewMatrix(lampShaderProgram, viewMatrix);
		setViewMatrix(objectShaderProgram, viewMatrix);
		setViewMatrix(carLampShaderProgram, viewMatrix);
		setViewMatrix(lightingShaderProgram2, viewMatrix);
		



	}
	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

