/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main ()
{
        color = u_color;
};
