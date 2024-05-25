#pragma once

#include "imstb_truetype.h"

#include <unordered_map>
#include "shader.h"

enum class TextAnchor
{
	TopLeft,
	TopCenter,
	TopRight,
	CenterLeft,
	Center,
	CenterRight,
	BottomLeft,
	BottomCenter,
	BottomRight
};

enum class TextJustification
{
	Left,
	Center,
	Right
};

class TextManager {
public:
	TextManager(glm::vec2 windowSize);

	void init(const char* fontPath, float fontSize, glm::vec3 position);

	~TextManager();
	
	void generateText(std::string text);
	void render();

	glm::vec3 m_position{};
	void loadFont(const char* fontPath);
private:

	struct TextMetrics
	{
		float maxCharHeight{};
		float fullTextHeight{};
		float fullTextWidth{};
	};

	void createTextureAtlas();
	void setupBuffers();

	GLuint textureAtlas;
	int atlasWidth = 512, atlasHeight = 512;
	std::vector<stbtt_bakedchar> charData{96}; // ASCII 32..126

	stbtt_fontinfo font;
	unsigned char* fontBuffer;

	unsigned int m_fontTexture{};
	unsigned int m_VAO{};
	unsigned int m_VBO{};

	TextMetrics m_textMetrics{};

	glm::vec2 m_windowSize{};

	TextAnchor m_anchor{TextAnchor::TopRight};
	TextJustification m_justification{TextJustification::Right};

	void calculateFullTextSize(const std::vector<std::string> lines);
	float getLineWidth(const std::string line);

	float m_fontSize{};


	std::vector<float> m_vertices;

	std::vector<std::string> splitTextIntoLines(const std::string& text);
};
