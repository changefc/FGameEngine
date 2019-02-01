#include "stdafx.h"
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Forward = glm::normalize(target - position);
	Right = glm::normalize(glm::cross(Forward, worldup));
	Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::Camera(glm::vec3 position, float pitch,float yaw, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	Forward.y = sin(glm::radians(pitch));
	Forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Right = glm::normalize(glm::cross(Forward, worldup));
	Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Forward, WorldUp);
}