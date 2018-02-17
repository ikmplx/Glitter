#include "stdafx.h"

#include "Shader.h"

#include <fstream>
#include <iostream>

static std::string ReadFile(const char* filename)
{
	std::ifstream in(filename);
	return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

static GLuint CreateShader(GLuint shaderType, const char* filename)
{
	GLuint shader = glCreateShader(shaderType);
	std::string sourceContent = ReadFile(filename);
	const char* sourceContentPtr = sourceContent.c_str();
	glShaderSource(shader, 1, &sourceContentPtr, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[2048];
		glGetShaderInfoLog(shader, 2048, NULL, infoLog);
		std::cout << "Shader compile failed:\n" << infoLog << std::endl;
	}

	return shader;
}



Shader::Shader()
{
}

Shader::Shader(const char* vertexFilename, const char* fragFilename)
{
	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexFilename);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragFilename);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[2048];
		glGetProgramInfoLog(shaderProgram, 2048, NULL, infoLog);
		std::cout << "Program link failed:\n" << infoLog << std::endl;
	}

	ID = shaderProgram;
}


Shader::~Shader()
{
	if (!ID) {
		return;
	}

	GLint id = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	if (id == ID) {
		glUseProgram(0);
	}

	glDeleteProgram(ID);
}

void Shader::Use()
{
	if (ID) {
		glUseProgram(ID);
	}
}

GLint Shader::GetUniformLocation(const char* id) const
{
	return glGetUniformLocation(ID, id);
}

void Shader::SetBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::SetInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat2(const std::string & name, float value1, float value2) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
}

void Shader::SetFloat3(const std::string & name, float value1, float value2, float value3) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

void Shader::SetVec3(const std::string & name, const glm::vec3& vec) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::SetMatrix(const std::string & name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
