#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include "stb_image.h"

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
unsigned int loadTexture(const char* path);

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


	//  ███████ ████████ ██    ██ ███████ ███████    Vertex data and textures
	//  ██         ██    ██    ██ ██      ██      
	//  ███████    ██    ██    ██ █████   █████   
	//       ██    ██    ██    ██ ██      ██      
	//  ███████    ██     ██████  ██      ██      
	//                                            


	float vertices[] = {
		 // positions       // normal
		 3.f, -1.5f, -3.f,  0.f, 1.f, 0.f,  
		 3.f, -1.5f,  3.f,  0.f, 1.f, 0.f,
		-3.f, -1.5f,  3.f,  0.f, 1.f, 0.f,
		-3.f, -1.5f, -3.f,  0.f, 1.f, 0.f,
	};

	unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
	};                       

	float cube[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	// Coordinate matrixes
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view;
	view = camera.getViewMatrix();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
                                                                                                                                                                                          

	unsigned int texture1 = loadTexture("resources/wood.png");
	unsigned int texture2 = loadTexture("resources/wood_specular.png");
	unsigned int texture3 = loadTexture("resources/matrix.jpg");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture3);


	//  ███████ ██   ██  █████  ██████  ███████ ██████  ███████ 
	//  ██      ██   ██ ██   ██ ██   ██ ██      ██   ██ ██      
	//  ███████ ███████ ███████ ██   ██ █████   ██████  ███████ 
	//       ██ ██   ██ ██   ██ ██   ██ ██      ██   ██      ██ 
	//  ███████ ██   ██ ██   ██ ██████  ███████ ██   ██ ███████ 
	//                                                          


	// Shader for objects
	Shader myShader("shaders/vertex.glsl", "shaders/fragment.glsl");
	myShader.use();

	myShader.setMat4("model", model);
	myShader.setMat4("view", view);
	myShader.setMat4("projection" ,projection);
	myShader.setVec3("lightPosition", lightPosition);

	myShader.setFloat("material.shininess", 32.f);
	myShader.setInt("material.diffuse", 0);
	myShader.setInt("material.specular", 1);
	myShader.setInt("material.emission", 2);

	glm::vec3 light_color = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 ambient = 0.0f * light_color;
	glm::vec3 diffuse = 0.5f * light_color;
	glm::vec3 specular = 1.f * light_color;

	myShader.setVec3("light.ambient", ambient);
	myShader.setVec3("light.diffuse", diffuse);
	myShader.setVec3("light.specular", specular);

	// Second shader for light source
	Shader lightShader("shaders/vertex.glsl", "shaders/light.glsl");
	lightShader.use();

	lightShader.setMat4("model", model);
	lightShader.setMat4("view", view);
	lightShader.setMat4("projection", projection);

	// Generating VAO and VBO buffers
	unsigned int VBOs[2];
	unsigned int VAOs[2];
	unsigned int EBO;

	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(1, &EBO);

	// Spinning cube
	glBindVertexArray(VAOs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Plane with bricks
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(VAOs[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Setting the clear color
	glClearColor(0.f, 0.f, 0.f, 1.0f);


	//  ██████  ███████ ███    ██ ██████  ███████ ██████  ██ ███    ██  ██████  
	//  ██   ██ ██      ████   ██ ██   ██ ██      ██   ██ ██ ████   ██ ██       
	//  ██████  █████   ██ ██  ██ ██   ██ █████   ██████  ██ ██ ██  ██ ██   ███ 
	//  ██   ██ ██      ██  ██ ██ ██   ██ ██      ██   ██ ██ ██  ██ ██ ██    ██ 
	//  ██   ██ ███████ ██   ████ ██████  ███████ ██   ██ ██ ██   ████  ██████  
	//                                                                                                                                       

	float radius = 8.f;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Light source

		view = camera.getViewMatrix();
		glm::mat4 model = glm::mat4(1.f);
		model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
		lightPosition.x = sin((float)glfwGetTime()) * radius;
		lightPosition.z = cos((float)glfwGetTime()) * radius;
		model = glm::translate(model, lightPosition);

		//light_color.x = sin(glfwGetTime() * 2.f);
		//light_color.y = sin(glfwGetTime() * 0.7f);
		//light_color.z = sin(glfwGetTime() * 1.3f);

		lightShader.use();
		lightShader.setMat4("model", model);
		lightShader.setMat4("view", view);
		lightShader.setVec3("lightColor", light_color);

		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cube

		myShader.use();
		myShader.setMat4("model", glm::mat4(1.f));
		myShader.setMat4("view", view);
		myShader.setVec3("lightPos", lightPosition);
		myShader.setVec3("viewPos", camera.getPosition());
		
		glm::vec3 ambient = 0.2f * light_color;
		glm::vec3 diffuse = 0.5f * light_color;
		glm::vec3 specular = 1.f * light_color;

		myShader.setVec3("light.ambient", ambient);
		myShader.setVec3("light.diffuse", diffuse);
		myShader.setVec3("light.specular", specular);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

		model = glm::mat4(1.f);
		myShader.setMat4("model", model);
		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Plane
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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

unsigned int loadTexture(const char* path) {

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data) {
		GLenum format{};

		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D,  0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Failed to load texture with path: " << path << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}