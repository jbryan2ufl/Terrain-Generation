#include "shader.h"

Shader::Shader()
{
	ID=glCreateProgram();
}

void Shader::setupShader(const char* shaderPath, GLenum shaderType)
{
	std::string shaderCodeString;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCodeString = shaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		return;
	}

	unsigned int shaderID{};

	switch (shaderType)
	{
		case GL_VERTEX_SHADER:
			shaderID = vertex;
			break;
		case GL_FRAGMENT_SHADER:
			shaderID = fragment;
			break;
		case GL_GEOMETRY_SHADER:
			shaderID = geometry;
			break;
		case GL_COMPUTE_SHADER:
			shaderID = compute;
			break;
		default:
			break;
	}

	if (shaderID != 0)
	{
		glDeleteShader(shaderID);
	}
	
	const char* shaderCode{shaderCodeString.c_str()};

	glDetachShader(ID, shaderID);
	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);
	checkCompileErrors(shaderID, "SHADER");
	glAttachShader(ID, shaderID);
	glDeleteShader(shaderID);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

void Shader::use()
{
	glUseProgram(ID);
}
