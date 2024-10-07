#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "camera.h"
#include "importer.h"

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

float deltaTime = 0.f;
float lastTime = 0.f;

Camera camera(glm::vec3(0.f, 0.f, 3.f), 2.5f, 0.1f);
float lastX = 300, lastY = 300;
bool first = true;

glm::vec3 lightPosition = glm::vec3(0.f, 4.f, 0.f);

int success;
char infoLog[512];

// ##### Declarations #####

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void getDeltaTime();

int main() {

	// ##### OpenGL Setup #####

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to inicialize window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	//stbi_set_flip_vertically_on_load(true);

	Shader shader("shaders/vertex_model.glsl", "shaders/fragment_model.glsl");
	Model backpack("resources/m4/m4a1_s.fbx");

	// Setting the clear color
	glClearColor(1.f, 0.f, 0.f, 1.0f);


	//  ██████  ███████ ███    ██ ██████  ███████ ██████  ██ ███    ██  ██████  
	//  ██   ██ ██      ████   ██ ██   ██ ██      ██   ██ ██ ████   ██ ██       
	//  ██████  █████   ██ ██  ██ ██   ██ █████   ██████  ██ ██ ██  ██ ██   ███ 
	//  ██   ██ ██      ██  ██ ██ ██   ██ ██      ██   ██ ██ ██  ██ ██ ██    ██ 
	//  ██   ██ ███████ ██   ████ ██████  ███████ ██   ██ ██ ██   ████  ██████  
	//                                                                                                                                       

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Coordinate matrixes
		shader.use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 0.f, -0.5f));
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		shader.setMat4("model", model);

		glm::mat4 view = camera.getViewMatrix(); 
		shader.setMat4("view", view);

		glm::mat4 projection = glm::perspective(glm::radians(60.f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
		shader.setMat4("projection", projection);

		// Lighting
		shader.setVec3("dirLight.direction", 0.f, -1.f, 0.f);
		shader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		shader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		shader.setVec3("dirLight.specular", 1.f, 1.f, 1.f);

		backpack.draw(shader);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


//  ███████ ██    ██ ███    ██  ██████ ████████ ██  ██████  ███    ██ ███████ 
//  ██      ██    ██ ████   ██ ██         ██    ██ ██    ██ ████   ██ ██      
//  █████   ██    ██ ██ ██  ██ ██         ██    ██ ██    ██ ██ ██  ██ ███████ 
//  ██      ██    ██ ██  ██ ██ ██         ██    ██ ██    ██ ██  ██ ██      ██ 
//  ██       ██████  ██   ████  ██████    ██    ██  ██████  ██   ████ ███████ 
//                                                                                                                                                        


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, WIDTH, HEIGHT);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	
	if (first) {
		lastX = xpos;
		lastY = ypos;
		first = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	
	camera.processMouse(xoffset, yoffset);
}

void getDeltaTime() {
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Camera movement
	getDeltaTime();
	const float cameraSpeed = 2.f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.processKeyboard(DOWN, deltaTime);
}