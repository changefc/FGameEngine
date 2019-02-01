// FGameEngine.cpp : �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

//Shader
#include "Shader.h"
#include "Material.h"

//ͼƬ����
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// camera
glm::vec3 cameraPos = glm::vec3(-4.8f, 3.5f, 1.4f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -2350.0f;
float pitch = -2412.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	//1��glfw Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create glfw window!");
		return -1;
	}

	//2�����Ӽ����¼�
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//3�����û���ģʽ
	//glEnable(GL_CULL_FACE);//�����޳�
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//����ģʽ ���(GL_FILL)������(GL_LINE)
	glEnable(GL_DEPTH_TEST);//������Ȼ��� ������һ����Ȼ���ͼ������Z�������������ͼAlphaֵ
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//4��glew Init
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Failed to init glew!");
		return -1;
	}
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//5������shader
	Shader* shader = new Shader("vertexfile.shader", "fragmentfile.shader");

	//6������ģ�͵�������
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  1.0f,  0.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 1,  // First Triangle
		1, 3, 2   // Second Triangle
	};

	//7������VAO VBO EBO
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//8����������
	stbi_set_flip_vertically_on_load(true);
	//8��1����Material
	Material* material = new Material(shader
		, glm::vec3(1.0f, 1.0f, 1.0f)
		, LoadImageToGPU("wall.jpg", GL_RGB, 0)
		, glm::vec3(0.0f, 0.5f, 0.0f)
		, 64);

	//GLuint texture;
	//glGenTextures(1, &texture);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//int widthTex, heightTex, nrChannel;
	//unsigned char* image = stbi_load("wall.jpg", &widthTex, &heightTex, &nrChannel, 0);
	//if (image)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	printf("load image fail!");
	//}
	//stbi_image_free(image);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//8.1 ��Ⱦ�ڶ���ͼ
	//GLuint textureFace;
	//glGenTextures(1, &textureFace);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, textureFace);
	//image = stbi_load("awesomeface.png", &widthTex, &heightTex, &nrChannel, 0);
	//if (image)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	printf("load image fail!");
	//}
	//stbi_image_free(image);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//8.2���þ���任
	glm::mat4 trans;

	//10����Ⱦ
	while (!glfwWindowShouldClose(window))
	{
		//1����ȡ��ʱ����
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//printf("Camer Pos X:%.1f  Y:%.1f  Z:%.1f  pitch:%.1f  yaw:%.1f pitchR:%.1f  yawR:%.1f Fov:%.1f \n",
		//	cameraPos.x, cameraPos.y, cameraPos.z, pitch, yaw, glm::radians(pitch), glm::radians(yaw),fov);

		//2������������������
		processInput(window);
		//����������ָ��
		glfwPollEvents();

		//3��������ת����
		//trans = glm::rotate(trans, glm::radians(0.005f), glm::vec3(1, 1, 0));

		//4����Ⱦ
		//4.1������������Ȼ���
		glClearColor(0.0f, 0.0f, 0.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//4.2������shader����
		shader->use();

		//4.3������������Ԫ����������
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture);
		//glUniform1i(glGetUniformLocation(shader->shaderProgram, "ourTexture"), 0);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, textureFace);
		//glUniform1i(glGetUniformLocation(shader->shaderProgram, "ourFace"), 1);

		//4.3������ǰ��VAO draw container
		glBindVertexArray(VAO);

		for (int i = 0; i < 10; i++)
		{
			//5.1������任�������
			unsigned int transformLoc = glGetUniformLocation(shader->shaderProgram, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

			//5.2�����û����������꼰����任�����������
			Camera camera(cameraPos,glm::radians(pitch), glm::radians(yaw), cameraUp);
			cameraFront = camera.Forward;
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0, 0));
			model = glm::translate(model, cubePositions[i]);
			view = camera.GetViewMatrix();
			projection = glm::perspective(glm::radians(fov), (float)width / height, 0.1f, 100.0f);

			unsigned int modelMat = glGetUniformLocation(shader->shaderProgram, "modelMat");
			unsigned int viewMat = glGetUniformLocation(shader->shaderProgram, "viewMat");
			unsigned int projectMat = glGetUniformLocation(shader->shaderProgram, "projectionMat");
			glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectMat, 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3f(glGetUniformLocation(shader->shaderProgram, "objectColor"), 1.0f, 0.5f, 0.31f);
			glUniform3f(glGetUniformLocation(shader->shaderProgram, "ambientColor"), 0.1f, 0.1f, 0.1f);
			glUniform3f(glGetUniformLocation(shader->shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(shader->shaderProgram, "lightPos"), 10.0f, 10.0f, 10.0f);
			glUniform3f(glGetUniformLocation(shader->shaderProgram, "viewPos"), cameraPos.x,cameraPos.y,cameraPos.z);

			shader->SetUniform3f("material.ambient", material->ambient);
			shader->SetUniformli("material.diffuse", material->diffuse,0);
			shader->SetUniform3f("material.specular", material->specular);
			glUniform1f(glGetUniformLocation(shader->shaderProgram, "material.shininess"), material->shininess);

			//5.3������
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//6�����VAO
		glBindVertexArray(0);

		//7����Ⱦ��������
		glfwSwapBuffers(window);
	}

	//8������ʼ��
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 2.0f; 
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= yoffset;
}

unsigned int LoadImageToGPU(char* imageName, GLenum type,int index) 
{
	stbi_set_flip_vertically_on_load(true);
	GLuint texture;
	glGenTextures(1, &texture);
	std::cout << GL_TEXTURE0 << '\n';
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	int widthTex, heightTex, nrChannel;
	unsigned char* image = stbi_load(imageName, &widthTex, &heightTex, &nrChannel, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, type, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("load image fail!");
	}
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, index);
	return texture;
}