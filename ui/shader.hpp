/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <string>

#include <glm/glm.hpp>

#define STD_VS "ui/res/shaders/cube.vs"
#define STD_FS "ui/res/shaders/cube.fs"

#define TXT_VS "ui/res/shaders/text.vs"
#define TXT_FS "ui/res/shaders/text.fs"

class Shader
{
public:
	Shader(const std::string& vsPath, const std::string& fsPath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string &name, int data);
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