#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID{};
	unsigned int vertex{};
	unsigned int fragment{};
	unsigned int geometry{};
	unsigned int compute{};

	Shader();
	void setupShader(const char* shaderPath, GLenum shaderType);
	void use();
	void checkCompileErrors(unsigned int, std::string);
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setFloat(const std::string &name, float f) const;
	void setInt(const std::string &name, int i) const;
	void setVec3(const std::string &name, const glm::vec3 &vec) const;
	void setVec4(const std::string &name, const glm::vec4 &vec) const;
};