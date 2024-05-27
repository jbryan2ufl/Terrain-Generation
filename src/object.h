#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <limits>
#include <memory>

#include "shader.h"
#include "windowData.h"
#include "modelMatrix.h"

class Object
{
public:
	unsigned int m_VAO{};
	unsigned int m_VBO{};
	unsigned int m_colorVBO{};

	std::vector<glm::vec3> m_data{};
	std::vector<glm::vec4> m_color_data{};

	glm::vec4 m_color{1, 1, 1, 1};

	Shader m_shader{};

	std::shared_ptr<WindowData> m_windowData{};

	ModelMatrix m_modelMatrix{};

	Object();

	void init(std::shared_ptr<WindowData> w);
	
	void render();

private:
};

#endif