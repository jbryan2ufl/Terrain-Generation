#include "SimpleTerrain.h"

SimpleTerrain::SimpleTerrain(Light& l, Camera& c)
    : m_light{l}
    , m_camera{c}
{
    m_vertices.reserve(m_width * m_height);
    m_indices.reserve((m_width - 1) * (m_height - 1) * 6);
    noiseData.reserve(m_width * m_height * 4);
    rawNoiseData.reserve(m_width * m_height * m_depth);

    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            for (int z = 0; z < m_depth; ++z)
            {
                float nx = static_cast<float>(x) / m_width;
                float ny = static_cast<float>(y) / m_height;
                float nz = static_cast<float>(z) / m_depth;

                float noiseValue = stb_perlin_noise3(nx * m_noiseDensity, ny * m_noiseDensity, nz * m_noiseDensity, 0, 0, 0);
                // float noiseValue = stb_perlin_noise3(nx * m_noiseDensity, ny * m_noiseDensity, nz * m_noiseDensity, 0, std::pow(2, m_noiseWrapPower), 0);

                rawNoiseData.push_back(noiseValue);
            }
        }
    }
}

void SimpleTerrain::init(std::shared_ptr<WindowData> w)
{
    m_windowData = w;
    setupBuffers(1);
    initGeometry();
    populateVAO();
}

void SimpleTerrain::renderSpecifics()
{
    glDrawArrays(GL_TRIANGLES, 0, m_trivertices.size());
}

void SimpleTerrain::setUniforms()
{
    m_modelMatrix.updateAll();
    m_shader->setUniform("model", m_modelMatrix.m_matrix);
    m_shader->setUniform("view", m_windowData->m_view);
    m_shader->setUniform("projection", m_windowData->m_perspective);

    m_shader->setUniform("materialValues", m_material.materialValues);
    m_shader->setUniform("objectColor", m_material.color);
    m_shader->setUniform("lightPos", m_light.lightPos);
    m_shader->setUniform("lightColor", m_light.lightColor);
    m_shader->setUniform("viewPos", m_camera.m_position);
    
    m_shader->setUniform("constant", 1.0f);
    m_shader->setUniform("linear", 0.005f);
    m_shader->setUniform("quadratic", 0.0001f);
}

void SimpleTerrain::populateVAO()
{
glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, m_trivertices.size() * sizeof(Vertex), &m_trivertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void SimpleTerrain::calculateNormals()
{
    std::vector<glm::vec3> faceNormals(m_indices.size() / 3);

    // Calculate normals for each face
    for (size_t i = 0; i < m_indices.size(); i += 3) {
        Vertex& v0 = m_vertices[m_indices[i]];
        Vertex& v1 = m_vertices[m_indices[i + 1]];
        Vertex& v2 = m_vertices[m_indices[i + 2]];

        glm::vec3 normal = glm::normalize(glm::cross(v1.m_position - v0.m_position, v2.m_position - v0.m_position));
        faceNormals[i / 3] = normal;

        // Accumulate normals in each vertex
        v0.m_normal += normal;
        v1.m_normal += normal;
        v2.m_normal += normal;
    }

    for (auto& vertex : m_vertices) {
        vertex.m_normal = glm::normalize(vertex.m_normal);
    }
}

void SimpleTerrain::generateVertices()
{
    for (const auto& idx : m_indices) {
        m_trivertices.push_back(m_vertices[idx]);
    }
}

void SimpleTerrain::regenerate()
{

    terrainV %= 128;
    
    m_modelMatrix.m_position = glm::vec3{0.0f};
    m_modelMatrix.updateAll();

    m_trivertices.clear();
    m_vertices.clear();
    m_indices.clear();
    noiseData.clear();

    float timeBefore{glfwGetTime()};

    for (int z = 0; z < m_height; ++z) {
        for (int x = 0; x < m_width; ++x) {
            float nx = static_cast<float>(x) / m_width;
            float nz = static_cast<float>(z) / m_height;

            float value = rawNoiseData[x + m_width * (terrainV + m_height * z)];
            // float value = 1;
            // if (v1) value *= stb_perlin_ridge_noise3(nx, terrainV, nz, terrainLacunarity, terrainGain, terrainOffset, terrainOctaves);
            // if (v1) value *= 1.0f;
            
            // if (v2) value *= stb_perlin_noise3(nx*m_noiseDensity, terrainV, nz*m_noiseDensity, 0, std::pow(2,m_noiseWrapPower), 0);

            glm::vec3 position{x, value * m_yscale, z};
            glm::vec3 normal{0.0f, 1.0f, 0.0f};

            m_vertices.emplace_back(position, normal);

            unsigned char color = static_cast<unsigned char>((value + 1.0f) * 0.5f * 255.0f);
            int index = (z * m_width + x) * 4;
            noiseData[index + 0] = color;
            noiseData[index + 1] = color;
            noiseData[index + 2] = color;
            noiseData[index + 3] = 255;

            if (x < m_width - 1 && z < m_height - 1) {
                int start = z * m_width + x;

                m_indices.push_back(start);
                m_indices.push_back(start + m_width);
                m_indices.push_back(start + 1);

                m_indices.push_back(start + 1);
                m_indices.push_back(start + m_width);
                m_indices.push_back(start + m_width + 1);
            }
        }
    }

    // Calculate normals
    calculateNormals();

    // Generate triangle vertices from indices
    generateVertices();
    float timeAfter{glfwGetTime()};
    // std::cout << "TIME TAKEN: " << timeAfter - timeBefore << '\n';

    glBindTexture(GL_TEXTURE_2D, perlinTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, noiseData.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    populateVAO();
}

void SimpleTerrain::initGeometry()
{
    glGenTextures(1, &perlinTexture);
    regenerate();
}