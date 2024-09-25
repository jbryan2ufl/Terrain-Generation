#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct Light
{
    glm::vec3 lightPos{0.0f, 10.0f, 0.0f};
    glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
};

#endif