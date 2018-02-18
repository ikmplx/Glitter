#include "stdafx.h"

#include "Shader.h"

#include <fstream>
#include <iostream>

namespace {
	std::string ReadFile(const char* filename)
	{
		std::ifstream in(filename);
		return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
	}

	GLuint CreateShader(GLuint shaderType, const char* filename)
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
}

namespace MyGL
{
	Shader::Shader(const std::string& name, const std::string& vertFilename, const std::string& fragFilename)
		: Resource(name, ResourceType::Shader)
	{
		GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertFilename.c_str());
		GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragFilename.c_str());

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

	void Shader::Bind()
	{
		if (ID) {
			glUseProgram(ID);
		}
	}

	GLint Shader::GetUniformLocation(const std::string& id) const
	{
		return glGetUniformLocation(ID, id.c_str());
	}

	GLint Shader::GetUniformLocation(const char* id) const
	{
		return glGetUniformLocation(ID, id);
	}

	void Shader::SetBool(const std::string & name, bool value) const
	{
		glUniform1i(GetUniformLocation(name), (int)value);
	}

	void Shader::SetInt(const std::string & name, int value) const
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetFloat(const std::string & name, float value) const
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetFloat2(const std::string & name, float value1, float value2) const
	{
		glUniform2f(GetUniformLocation(name), value1, value2);
	}

	void Shader::SetFloat3(const std::string & name, float value1, float value2, float value3) const
	{
		glUniform3f(GetUniformLocation(name), value1, value2, value3);
	}

	void Shader::SetVec3(const std::string & name, const glm::vec3& vec) const
	{
		glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
	}

	void Shader::SetMatrix(const std::string & name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}
}