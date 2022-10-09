#include <iostream>
#include <fstream>
#include <string>

#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "texture.hpp"

Texture::Texture(const std::string &path)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file)
    {
        std::cerr << "Cannot open file: " << path << std::endl;
		return;
	}

    BMPheader header;
    file.read((char*)&header, sizeof(header));
    if (header.type != 0x4D42)
    {
        std::cerr << "File: " << path << " is not a BMP file." << std::endl;
    }

    int channels = header.BPP / 8;
    if (header.imageSize == 0)
    {
        header.imageSize = header.width * header.height * channels;
    }

    char data[header.imageSize];
    file.read(data, header.imageSize);
    file.close();

    int format;
    switch (channels)
    {
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default:
            std::cerr << "Invalid texture format" << std::endl; break;
    }

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, format, header.width, header.height,
        0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}