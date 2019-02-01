#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(glm::vec3 position,glm::vec3 target,glm::vec3 worldup);
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
	glm::vec3 Position;
	glm::vec3 WorldUp;
	glm::vec3 Forward;
	glm::vec3 Right;
	glm::vec3 Up;
	~Camera();
	
	//����ռ�-�۲�ռ�任����
	glm::mat4 GetViewMatrix();
};

