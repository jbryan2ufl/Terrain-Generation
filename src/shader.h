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

	template <typename T>
    void setUniform(const std::string &name, const T &value) const {
        int location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            // Handle error: uniform not found
            return;
        }

        setUniformValue(location, value);
    }

private:
	void setUniformValue(int location, float f) const { glUniform1f(location, f); }
    void setUniformValue(int location, int i) const { glUniform1i(location, i); }
    void setUniformValue(int location, const glm::vec2 &vec) const { glUniform2f(location, vec.x, vec.y); }
    void setUniformValue(int location, const glm::vec3 &vec) const { glUniform3f(location, vec.x, vec.y, vec.z); }
    void setUniformValue(int location, const glm::vec4 &vec) const { glUniform4f(location, vec.x, vec.y, vec.z, vec.w); }
    void setUniformValue(int location, const glm::mat4 &mat) const { glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]); }
    void setUniformValue(int location, const std::vector<glm::vec3> &vecArray) const { glUniform3fv(location, vecArray.size(), &vecArray[0][0]); }
};