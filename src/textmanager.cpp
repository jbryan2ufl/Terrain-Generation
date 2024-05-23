#include "textmanager.h"

TextManager::TextManager()
{
}

void TextManager::loadFont(const char* fontPath)
{
	FILE* fontFile = fopen(fontPath, "rb");
	if (!fontFile) {
		std::cerr << "Could not open font file: " << fontPath << std::endl;
		return;
	}

	fseek(fontFile, 0, SEEK_END);
	size_t fontFileSize = ftell(fontFile);
	fseek(fontFile, 0, SEEK_SET);

	fontBuffer = new unsigned char[fontFileSize];
	fread(fontBuffer, fontFileSize, 1, fontFile);
	fclose(fontFile);

	stbtt_InitFont(&font, fontBuffer, stbtt_GetFontOffsetForIndex(fontBuffer, 0));
}

void TextManager::createTextureAtlas()
{
	unsigned char* bitmap = new unsigned char[atlasWidth * atlasHeight];
	stbtt_BakeFontBitmap(fontBuffer, 0, 32.0, bitmap, atlasWidth, atlasHeight, 32, 96, &charData[0]);

	glGenTextures(1, &textureAtlas);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

	// Check for OpenGL errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "glTexImage2D error: " << error << std::endl;
	}

	// Generate mipmaps for the texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// Check for OpenGL errors
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "glGenerateMipmap error: " << error << std::endl;
	}

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Check for OpenGL errors
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "glTexParameteri error: " << error << std::endl;
	}

	delete[] bitmap;
}

void TextManager::setupBuffers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

std::vector<std::string> TextManager::splitTextIntoLines(const std::string& text)
{
	std::vector<std::string> lines;
	std::istringstream stream{text};
	std::string line;
	while (std::getline(stream, line))
	{
		lines.push_back(line);
	}
	return lines;
}

void TextManager::init(const char* fontPath, float fontSize)
{
	loadFont(fontPath);
	createTextureAtlas();
	setupBuffers(); 
}

TextManager::~TextManager()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void TextManager::generateText(std::string text, float x, float y, float scale)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);
	glBindVertexArray(m_VAO);

	m_vertices.clear();

	float startX = x;
	float startY = y;
	float lineHeight = 32.0f;  // Adjust line height as necessary

	auto lines = splitTextIntoLines(text);
	for (const auto& line : lines) {
		x = startX;
		for (char c : line) {
			if (c >= 32 && c < 128) {
				stbtt_aligned_quad q;
				stbtt_GetBakedQuad(&charData[0], atlasWidth, atlasHeight, c - 32, &x, &y, &q, 1);

				float y0 = (1.0f - q.y0) * scale;
				float y1 = (1.0f - q.y1) * scale;

				float x0 = q.x0 * scale;
				float x1 = q.x1 * scale;

				m_vertices.insert(m_vertices.end(), {
					x0, y0, q.s0, q.t0,
					x1, y0, q.s1, q.t0,
					x1, y1, q.s1, q.t1,

					x0, y0, q.s0, q.t0,
					x1, y1, q.s1, q.t1,
					x0, y1, q.s0, q.t1
				});
			}
		}
		y += lineHeight;  // Move to the next line
	}
}

void TextManager::render()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 4);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}
