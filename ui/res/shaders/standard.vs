/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#version 330 core

layout(location = 0) in vec3 position;
uniform mat4 MVP;

void main()
{
        gl_Position = MVP * vec4(position, 1);
};
