/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
uniform mat4 MVP;

out vec2 UV;

void main()
{
        gl_Position = MVP * vec4(position, 1);
        UV = vertexUV;
};
