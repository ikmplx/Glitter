#include "stdafx.h"

#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {
	std::string ReadFile(const char* filename)
	{
		std::ifstream in(filename);
		return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
	}

	std::string ReadShaderFile(const char* filename)
	{
		static const char importLine[] = "@include ";

		std::ifstream in(filename);

		std::ostringstream out;

		int nLine = 0;
		std::string line;
		while (std::getline(in, line)) {
			nLine++;

			if (line.find(importLine, 0) == 0) {
				std::string includeName = "Shaders/Include/" + line.substr(sizeof(importLine) - 1) + ".glsl";
				std::string includeContent = ReadFile(includeName.c_str());

				out << "// begin included from " << includeName << '\n';
				out << includeContent << '\n';
				out << "// end included from " << includeName << '\n';
				out << "#line " << (nLine + 1) << "\n";
			}
			else {
				out << line << '\n';
			}
		}

		return out.str();
	}

	GLuint CreateShader(GLuint shaderType, const char* filename)
	{
		if (!filename || filename[0] == '\0') {
			return 0;
		}

		GLuint shader = glCreateShader(shaderType);
		std::string sourceContent = ReadShaderFile(filename);
		const char* sourceContentPtr = sourceContent.c_str();
		glShaderSource(shader, 1, &sourceContentPtr, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GLchar infoLog[2048];
			glGetShaderInfoLog(shader, 2048, NULL, infoLog);
			std::cout << "Shader compile failed: " << filename << ":\n" << infoLog << std::endl;
		}

		return shader;
	}
}

namespace MyGL
{
	namespace UboManager
	{
		static GLuint ubos[3];

		void Initialize()
		{
			// Matrices
			glGenBuffers(3, ubos);

			glBindBuffer(GL_UNIFORM_BUFFER, ubos[BINDING_MATRICES]);
			glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_MATRICES, ubos[BINDING_MATRICES], 0, 3 * sizeof(glm::mat4));

			// MatricesExt
			glBindBuffer(GL_UNIFORM_BUFFER, ubos[BINDING_MATRICES_EXT]);
			glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_MATRICES_EXT, ubos[BINDING_MATRICES_EXT], 0, 3 * sizeof(glm::mat4));

			// Vectors
			glBindBuffer(GL_UNIFORM_BUFFER, ubos[BINDING_VECTORS]);
			glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_VECTORS, ubos[BINDING_VECTORS], 0, 1 * sizeof(glm::vec4));

		}

		void SetMatrix(GLuint binding, int matrixIndex, const glm::mat4& mat)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, ubos[binding]);
			glBufferSubData(GL_UNIFORM_BUFFER, matrixIndex * sizeof(glm::mat4), sizeof(glm::mat4), &mat);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void SetVector(GLuint binding, int vectorIndex, const glm::vec4& vector)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, ubos[binding]);
			glBufferSubData(GL_UNIFORM_BUFFER, vectorIndex * sizeof(glm::vec4), sizeof(glm::vec4), &vector);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void Deinitialize()
		{
			glDeleteBuffers(2, ubos);
		}

		void SetupShader(GLuint shaderId)
		{
			// Matrices
			GLuint uboIndex = glGetUniformBlockIndex(shaderId, "Matrices");
			if (uboIndex != GL_INVALID_INDEX) {
				glUniformBlockBinding(shaderId, uboIndex, BINDING_MATRICES);
			}

			// MatricesExt
			uboIndex = glGetUniformBlockIndex(shaderId, "MatricesExt");
			if (uboIndex != GL_INVALID_INDEX) {
				glUniformBlockBinding(shaderId, uboIndex, BINDING_MATRICES_EXT);
			}

			// Vectors
			uboIndex = glGetUniformBlockIndex(shaderId, "Vectors");
			if (uboIndex != GL_INVALID_INDEX) {
				glUniformBlockBinding(shaderId, uboIndex, BINDING_VECTORS);
			}
		}
	}


	Shader::Shader(const std::string& name, const std::string& vertFilename, const std::string& fragFilename, const std::string& geomFilename)
		: Resource(name, ResourceType::Shader)
	{
		GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertFilename.c_str());
		GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragFilename.c_str());
		GLuint geometryShader = CreateShader(GL_GEOMETRY_SHADER, geomFilename.c_str());

		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		if (geometryShader) {
			glAttachShader(shaderProgram, geometryShader);
		}

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryShader) {
			glDeleteShader(geometryShader);
		}

		GLint success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		if (!success)
		{
			GLchar infoLog[2048];
			glGetProgramInfoLog(shaderProgram, 2048, NULL, infoLog);
			std::cout << "Program link failed:\n" << infoLog << std::endl;
		}
		else {
			UboManager::SetupShader(shaderProgram);
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