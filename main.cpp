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

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

float deltaTime = 0.f;
float lastTime = 0.f;

Camera camera(glm::vec3(0.f, 0.f, 3.f),  2.5f, 0.1f);
float lastX = 300, lastY = 300;
bool first = true;

glm::vec3 lightPosition = glm::vec3(-0.3f, -0.3f, 2.7f);

int success;
char infoLog[512];

bool B_pressed = false;
bool wireframe = false;

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

	//     _____ _______ _    _ ______ ______ 
	//    / ____|__   __| |  | |  ____|  ____|
	//   | (___    | |  | |  | | |__  | |__   
	//    \___ \   | |  | |  | |  __| |  __|  
	//    ____) |  | |  | |__| | |    | |     
	//   |_____/   |_|   \____/|_|    |_|     
	//                                        
	//                                        

	Shader material_shader("shaders/vertex_model.glsl", "shaders/fragment_model.glsl");
	Shader wireframe_shader("shaders/vertex_model.glsl", "shaders/wireframe.glsl");
	Shader light_shader("shaders/vertex.glsl", "shaders/light.glsl");

	// M4A1s setup
	Model m4a1("resources/m4a1s/corm4.obj");
	material_shader.use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.1f, -0.08f, 2.7f));
	model = glm::scale(model, glm::vec3(2.f));
	material_shader.setMat4("model", model);

	wireframe_shader.use();
	wireframe_shader.setMat4("model", model);

	// Light
	Cube cube(lightPosition, glm::vec3(1.f, 0.f, 1.f));

	// Coordinate matrixes
	material_shader.use();

	// Lighting
	glm::vec3 light_color = cube.get_color();
	glm::vec3 ambient = 0.1f * light_color;
	glm::vec3 diffuse = 0.5f * light_color;
	glm::vec3 specular = 1.0f * light_color;

	material_shader.setFloat("material.shininess", 64.0);
	material_shader.setVec3("dirLight.direction", 0.f, -1.f, (float)sin(glfwGetTime()));
	material_shader.setVec3("dirLight.ambient", glm::vec3(0.2f));
	material_shader.setVec3("dirLight.diffuse", glm::vec3(0.5f));
	material_shader.setVec3("dirLight.specular", glm::vec3(1.f));

	material_shader.setVec3("pointLight.ambient", ambient);
	material_shader.setVec3("pointLight.diffuse", diffuse);
	material_shader.setVec3("pointLight.specular", specular);

	material_shader.setFloat("pointLight.constant", 1.f);
	material_shader.setFloat("pointLight.linear", 0.09f);
	material_shader.setFloat("pointLight.quadratic", 0.032f);
	material_shader.setVec3("pointLight.position", cube.get_position());

	// Setting the clear color
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	//    _____  ______ _   _ _____  ______ _____  
	//   |  __ \|  ____| \ | |  __ \|  ____|  __ \ 
	//   | |__) | |__  |  \| | |  | | |__  | |__) |
	//   |  _  /|  __| | . ` | |  | |  __| |  _  / 
	//   | | \ \| |____| |\  | |__| | |____| | \ \ 
	//   |_|  \_\______|_| \_|_____/|______|_|  \_\
	//                                             
	//                                             

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		material_shader.use();
		if (wireframe)
			wireframe_shader.use();

		glm::mat4 view = camera.getViewMatrix(); 
		material_shader.setMat4("view", view);

		glm::mat4 projection = glm::perspective(glm::radians(60.f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
		material_shader.setMat4("projection", projection);
		material_shader.setVec3("viewPos", camera.getPosition());

		wireframe_shader.setMat4("view", view);
		wireframe_shader.setMat4("projection", projection);
		wireframe_shader.setVec3("viewPos", camera.getPosition());

		m4a1.draw(material_shader);

		// Light cube
		light_shader.use();
		light_shader.setMat4("view", view);
		light_shader.setMat4("projection", projection);
		cube.draw(light_shader);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

	//    ______ _    _ _   _  _____ _______ _____ ____  _   _  _____ 
	//   |  ____| |  | | \ | |/ ____|__   __|_   _/ __ \| \ | |/ ____|
	//   | |__  | |  | |  \| | |       | |    | || |  | |  \| | (___  
	//   |  __| | |  | | . ` | |       | |    | || |  | | . ` |\___ \ 
	//   | |    | |__| | |\  | |____   | |   _| || |__| | |\  |____) |
	//   |_|     \____/|_| \_|\_____|  |_|  |_____\____/|_| \_|_____/ 
	//                                                                
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

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		if (!B_pressed) {
			wireframe = !wireframe;

			if (wireframe) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			B_pressed = true;
		}
	}
	else {
		B_pressed = false;
	}

	// Camera movement
	getDeltaTime();
	const float cameraSpeed = 3.f;
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