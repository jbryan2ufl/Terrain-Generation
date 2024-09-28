#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

#include "shader.h"
#include "windowData.h"

class ShaderManager
{
public:
	static ShaderManager& getInstance()
	{
		static ShaderManager instance;
		return instance;
	}

	template<typename... Args>
	std::shared_ptr<Shader> loadNewShader(const std::string& s, Args... args);

	template<typename... Args>
	void compileShaders(Shader& shader, const char* path, GLenum shaderType, Args... args);
	void compileShaders(Shader& shader){}

	std::shared_ptr<Shader> getShader(const std::string& s);

	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;

private:
	ShaderManager()
	{}
	

	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders{};
};

template<typename... Args>
std::shared_ptr<Shader> ShaderManager::loadNewShader(const std::string& s, Args... args)
{
	if (m_shaders.find(s) != m_shaders.end()) {
		return m_shaders[s];
	}

	auto shader = std::make_shared<Shader>();

	compileShaders(*shader, args...);

	m_shaders[s] = shader;
	return shader;
}

template<typename... Args>
void ShaderManager::compileShaders(Shader& shader, const char* path, GLenum shaderType, Args... args)
{
	shader.setupShader(path, shaderType);
	compileShaders(shader, args...);
}

#endif
