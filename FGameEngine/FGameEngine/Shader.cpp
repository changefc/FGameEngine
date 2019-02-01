#include "stdafx.h"
#include "Shader.h"
#include <fstream>
#include <sstream>

using namespace std;

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	//1、读取Shader文件
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

	//2、创建sharder
	GLint success;
	GLchar infoLog[512];
	//2.1、创建点shader
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
	if (shaderProgram != NULL)
	{
		glUseProgram(shaderProgram);
	}
	else
	{
		std::cout << "ERROR::ShaderProgram is null!\n" << std::endl;
	}
	
}

void Shader::SetUniform3f(const char* paramName, glm::vec3 param)
{
	if (shaderProgram != NULL)
	{
		glUniform3f(glGetUniformLocation(shaderProgram, paramName), param.x, param.y, param.z);
	}
	else
	{
		std::cout << "ERROR::ShaderProgram is null!\n" << std::endl;
	}
}

void Shader::SetUniform1f(const char * paramName, float param)
{
	if (shaderProgram != NULL)
	{
		glUniform1f(glGetUniformLocation(shaderProgram, paramName), param);
	}
	else
	{
		std::cout << "ERROR::ShaderProgram is null!\n" << std::endl;
	}
}

void Shader::SetUniformli(const char* paramName, unsigned int param,int index)
{
	if (shaderProgram != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, param);
		glUniform1i(glGetUniformLocation(shaderProgram, paramName), index);
	}
	else
	{
		std::cout << "ERROR::ShaderProgram is null!\n" << std::endl;
	}
}
