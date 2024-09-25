#ifndef TERRAIN_H
#define TERRAIN_H

#include "vertex.h"
#include "renderable.h"
#include "stb_perlin.h"
#include "material.h"
#include "light.h"
#include "camera.h"

class SimpleTerrain : public Renderable
{
public:
	SimpleTerrain(Light& l, Camera& c);

	std::vector<Vertex> m_vertices{};
	std::vector<Vertex> m_trivertices{};
	std::vector<unsigned int> m_indices{};

	unsigned int m_height{128};
	unsigned int m_width{128};
	unsigned int m_depth{128};
	float m_yscale{100.0f};
	float m_noiseDensity{1.0f};
	int m_noiseWrapPower{0};


	void init(std::shared_ptr<WindowData> w) override;
	void renderSpecifics() override;
	void setUniforms() override;
	void populateVAO() override;
	void initGeometry() override;


	void calculateNormals();
	void generateVertices();

	Material m_material{};
	Light& m_light;
	Camera& m_camera;

	int terrainV{0};
	float terrainLacunarity{2.0f};
	float terrainGain{0.5};
	float terrainOffset{1.0};
	int terrainOctaves{6};

	bool v1{true};
	bool v2{true};

	void regenerate();

	std::vector<float> rawNoiseData;
	std::vector<unsigned char> noiseData;
	unsigned int perlinTexture{};
};

#endif