#include "modelMatrix.h"

ModelMatrix::ModelMatrix()
{
}


void ModelMatrix::updateTranslation()
{
}

void ModelMatrix::updateScale()
{
}

void ModelMatrix::updateRotation()
{

}

void ModelMatrix::updateAll()
{
	m_matrix = glm::mat4{1.0f};
	m_matrix = glm::translate(m_matrix, m_position);
	m_matrix = glm::scale(m_matrix, glm::vec3{m_scaleFactor});
	m_matrix = glm::rotate(m_matrix, m_angle, m_axis);
}
