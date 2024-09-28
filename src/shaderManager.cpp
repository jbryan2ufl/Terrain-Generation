#include "shaderManager.h"

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& s)
{
	if (m_shaders.find(s) == m_shaders.end())
	{
		std::cout << "Shader with handle " << s << " does not exist.\n";
		return nullptr;
	}
	
	return m_shaders[s];
}
