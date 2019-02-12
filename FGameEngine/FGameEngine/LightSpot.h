#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

class LightSpot
{
public:
	LightSpot(glm::vec3 _angles, glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f),float _cutAngle = 45.0f);
	~LightSpot();

	glm::vec3 position;
	glm::vec3 angles;
	glm::vec3 color;
	glm::vec3 direction;

	float constant;
	float linear;
	float quadratic;

	float cutAngle;
	float cosPhy;

	void UpdateDirection();
};

