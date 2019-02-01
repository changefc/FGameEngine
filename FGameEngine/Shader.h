#pragma once
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

class Shader
{
public:
	Shader(const char* vertexFile,const char* fragmentFile);
	GLuint shaderProgram;
	void use();

private:
	
};

