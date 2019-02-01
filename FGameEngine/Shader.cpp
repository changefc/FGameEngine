#include "stdafx.h"
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	const GLchar* vShaderCode;
	const GLchar* fShaderCode;

	vShaderFile.open(vertexFile);
	fShaderFile.open(fragmentFile);

	try
	{
		if (!vShaderFile.is_open() || !fShaderFile.is_open())
		{
			throw exception("open fiile error");
		}

		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (const std::exception &ex)
	{
		printf(ex.what());
	}

	vShaderCode = vertexCode.c_str();
	fShaderCode = fragmentCode.c_str();

	//3、创建sharder
	GLint success;
	GLchar infoLog[512];
	//3.1、创建点shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::VERTEX\n" << infoLog << std::endl;
	}

	//3.2、创建面shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::FRAGMENT\n" << infoLog << std::endl;
	}

	//3.3、连接Shader
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::use()
{
	//设置Uniform输入
	//GLfloat timeValue = glfwGetTime();
	//GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
	//GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "outColor");

	//渲染三角形
	//glUseProgram(shaderProgram);
	//CPU动态设置颜色值
	//glUniform4f(vertexColorLocation, 0, greenValue, 0, 1);

	glUseProgram(shaderProgram);
}