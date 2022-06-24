#pragma once

#include <string>

#include <glm/glm.hpp>

#define VERTEX_SHADER "ui/res/shaders/vertex.shader"
#define FRAGMENT_SHADER "ui/res/shaders/fragment.shader"

/*
unsigned int createShader (
	const std::string& vsPath,
	const std::string& fsPath);
*/

class Shader
{
public:
	Shader(const std::string& vsPath, const std::string& fsPath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform4f(const std::string& name,
		float r, float g, float b, float a);
	void setUniformMat4f(const std::string& name, const glm::mat4& mat);
private:
	unsigned int id;
	unsigned int getUniformLocation(const std::string& name);

	std::string parseShader(const std::string& path);
	unsigned int compileShader (unsigned int type,
		const std::string& source);
};