#pragma once

#include "imstb_truetype.h"

#include <unordered_map>
#include "shader.h"

class TextManager {
public:
	TextManager();

	void init(const char* fontPath, float fontSize);

	~TextManager();
	
	void generateText(std::string text, float x, float y, float scale);
	void render();

private:

	void loadFont(const char* fontPath);
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

	std::vector<float> m_vertices;

	std::vector<std::string> splitTextIntoLines(const std::string& text);
};
