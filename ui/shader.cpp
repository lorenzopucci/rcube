#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "shader.hpp"


std::string Shader::parseShader(const std::string& path)
{
    std::ifstream stream(path);

    std::string line;
    std::stringstream ss;
    while (getline(stream, line))
    {
        ss << line << '\n';
    }
    return ss.str();
}


unsigned int Shader::compileShader (
	unsigned int type,
	const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);

		char* msg = (char*)alloca(lenght * sizeof(char));
		glGetShaderInfoLog(id, lenght, &lenght, msg);

		std::cerr << "Failed to compile shader:\n" << msg << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, parseShader(vsPath));
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, parseShader(fsPath));

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	id = program;
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::bind() const
{
	glUseProgram(id);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name,
	float r, float g, float b, float a)
{
	glUniform4f(getUniformLocation(name), r, g, b, a);
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
	glGetUniformLocation(id, name.c_str());
}