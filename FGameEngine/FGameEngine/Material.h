#pragma once
//Shader
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
public:
	Shader *shader;
	glm::vec3 ambient;
	unsigned int diffuse;
	glm::vec3 specular;
	float shininess;

	Material(Shader* _shader,glm::vec3 _ambient, unsigned int _diffuse, glm::vec3 _specular,float _shininess);
	~Material();
};

