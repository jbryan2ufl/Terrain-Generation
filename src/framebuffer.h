#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

#include "shader.h"
#include "windowData.h"

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void initFBO(size_t width, size_t height);
	void bindFBO();
	void unbindFBO();
	unsigned int getTexture() const { return m_textureColorBuffer; }
    void render();
private:
    unsigned int m_fbo{};
	unsigned int m_textureColorBuffer{};
    std::shared_ptr<WindowData> m_windowData{};
    std::shared_ptr<Shader> m_downscaleShader{};
    std::shared_ptr<Shader> m_colorPaletteShader{};
    std::shared_ptr<Shader> m_ditherShader{};
};

#endif