#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <limits>

class Object
{
public:
	unsigned int m_VAO{};
	unsigned int m_VBO{};
	unsigned int m_colorVBO{};

	std::vector<glm::vec3> m_data{};
	std::vector<glm::vec4> m_color_data{};

	glm::vec4 m_color{1, 1, 1, 1};

	Object();

	void init();
	
	void render();

private:
};

#endif