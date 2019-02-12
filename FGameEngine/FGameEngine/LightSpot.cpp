#include "stdafx.h"
#include "LightSpot.h"


LightSpot::LightSpot(glm::vec3 _angles, glm::vec3 _position, glm::vec3 _color,float _cutInnerAngle, float _cutOutterAngle)
{
	position = _position;
	angles = _angles;
	color = _color;
	cutInnerAngle = _cutInnerAngle;
	cutOutterAngle = _cutOutterAngle;
	cosInnerPhy	= glm::cos(glm::radians(_cutInnerAngle / 2));
	cosOutterPhy = glm::cos(glm::radians(_cutOutterAngle / 2));

	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;

	UpdateDirection();
}


LightSpot::~LightSpot()
{
}

void LightSpot::UpdateDirection()
{
	direction = glm::vec3(0, 0, 1.0f);
	direction = glm::rotateZ(direction, angles.z);
	direction = glm::rotateX(direction, angles.x);
	direction = glm::rotateY(direction, angles.y);
	direction = -1.0f * direction;
}