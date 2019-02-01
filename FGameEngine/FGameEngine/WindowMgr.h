#pragma once
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class WindowMgr
{
public:
	WindowMgr(float width,float height);

	//function
	void Update();

	//param
	float width;
	float height;
	GLFWwindow* window;

	// camera
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	bool firstMouse;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;

	~WindowMgr();

	void call_back(GLFWwindow* window, int key, int scancode, int action, int mode);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow *window);

private:
	

	float deltaTime;
	float lastFrame;
};

