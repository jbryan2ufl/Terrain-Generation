#ifndef MODELMATRIX_H
#define MODELMATRIX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class ModelMatrix
{
public:
	ModelMatrix();
	void updateTranslation();
	void updateScale();
	void updateRotation();
	void updateAll();
	glm::mat4 m_matrix{1.0f};
	glm::mat4 m_translate{1.0f};
	glm::mat4 m_scale{1.0f};
	glm::mat4 m_rotate{1.0f};
	float m_angle{};
	glm::vec3 m_axis{0.0f, 0.0f, 1.0f};
	float m_scaleFactor{1.0f};
	glm::vec3 m_position{};
private:
};

#endif