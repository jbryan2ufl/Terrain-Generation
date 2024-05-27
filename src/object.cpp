#include "object.h"

Object::Object()
{
}

void Object::init(std::shared_ptr<WindowData> w)
{

	m_windowData = w;

	m_shader = Shader("../src/object.vs", "../src/object.fs");

	m_data.push_back(glm::vec3{-0.5f, -0.5f, 0.0f});
	m_data.push_back(glm::vec3{0.5f, -0.5f, 0.0f});
	m_data.push_back(glm::vec3{-0.5f, 0.5f, 0.0f});
	m_color_data.push_back(m_color);
	m_color_data.push_back(m_color);
	m_color_data.push_back(m_color);

	// for (auto& i : m_data)
	// {
	// 	glm::mat4 mvpMatrix {m_windowData->m_projection * m_windowData->m_view * m_modelMatrix.m_matrix};
	// 	glm::vec4 pos{i.x, i.y, 0.0f, 1.0f};
	// 	glm::vec4 finalPos{mvpMatrix * pos};

	// 	std::cout << finalPos.x << ' ' << finalPos.y << ' ' << finalPos.z << '\n';
	// }

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_colorVBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_data.size()*sizeof(glm::vec3), m_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, m_color_data.size()*sizeof(glm::vec4), m_color_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
}

void Object::render()
{
	m_shader.use();
	glm::mat4 mvpMatrix {m_windowData->m_perspective * m_windowData->m_view * m_modelMatrix.m_matrix};
	m_shader.setMat4("mvpMatrix", mvpMatrix);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_data.size());
	glBindVertexArray(0);
}