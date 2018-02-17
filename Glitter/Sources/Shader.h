#pragma once

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	Shader();
	~Shader();

	// Use the program
	void Use();

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

