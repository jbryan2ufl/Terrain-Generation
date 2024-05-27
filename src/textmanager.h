#pragma once

#include "imstb_truetype.h"
#include <unordered_map>
#include <memory>
#include "shader.h"
#include "modelMatrix.h"
#include "windowData.h"

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

enum class TextViewingMode
{
	Orthographic,
	Perspective
};

class TextManager
{
public:
	TextManager();

	void init(std::shared_ptr<WindowData> w, const char* fontPath, float fontSize, glm::vec3 position, const char* vs, const char* fs, TextViewingMode v, TextAnchor a, TextJustification j);

	~TextManager();

	void generateText(std::string text);
	void render();

	void loadFont(const char* fontPath);
	ModelMatrix m_modelMatrix{};

	void setJustification(TextJustification j);
	void setAnchor(TextAnchor a);

private:

	std::shared_ptr<WindowData> m_windowData{};

	std::string m_text{};

	TextViewingMode m_viewMode{TextViewingMode::Orthographic};

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
	Shader m_shader{};

	TextAnchor m_anchor{TextAnchor::Center};
	TextJustification m_justification{TextJustification::Center};

	void calculateFullTextSize(const std::vector<std::string> lines);
	float getLineWidth(const std::string line);

	float m_fontSize{};

	std::vector<float> m_vertices;

	std::vector<std::string> splitTextIntoLines(const std::string& text);
};
