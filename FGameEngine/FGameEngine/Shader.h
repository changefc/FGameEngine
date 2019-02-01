#pragma once
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(const char* vertexFile,const char* fragmentFile);
	GLuint shaderProgram;

	// π”√Shader
	void use();

	void SetUniform3f(const char* paramName, glm::vec3 param);
	void SetUniform1f(const char* paramName, float param);
	void SetUniformli(const char* paramName, unsigned int param, int index);
private:
	
};

