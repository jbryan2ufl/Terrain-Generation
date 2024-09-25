#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

struct Material
{
    glm::vec3 color{1.0f, 0.0f, 0.0f};
    glm::vec4 materialValues{0.1f, 0.5f, 0.1f, 64.0f}; // ambient, diffuse, specular, shininess
};

#endif