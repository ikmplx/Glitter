#pragma once

#include "Res.h"

namespace MyGL
{
	namespace UboManager
	{
		void Initialize();
		void Deinitialize();

		void SetupShader(GLuint shaderId);

		void SetMatrices(int matrixIndex, const glm::mat4& mat);
	};

	class Shader : public Resource
	{
	public:
		Shader(const std::string& name, const std::string& vertFilename, const std::string& fragFilename);
		~Shader();

		// Use the program
		void Bind();

		GLint GetUniformLocation(const std::string& id) const;
		GLint GetUniformLocation(const char* id) const;

		// utility uniform functions
		void SetBool(const std::string &name, bool value) const;
		void SetInt(const std::string &name, int value) const;
		void SetFloat(const std::string &name, float value) const;
		void SetFloat2(const std::string &name, float value1, float value2) const;
		void SetFloat3(const std::string &name, float value1, float value2, float value3) const;
		void SetMatrix(const std::string &name, const glm::mat4& mat) const;
		void SetVec3(const std::string & name, const glm::vec3& vec) const;

	public:
		// The program ID
		GLuint ID = 0;
	};
	using ShaderPtr = std::shared_ptr<Shader>;
}