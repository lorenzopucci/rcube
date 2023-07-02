/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D textureSampler;
uniform vec4 inColor;

void main ()
{
	if (UV.x == 0.0 && UV.y == 0.0)
	{
		color = vec4(0, 0, 0, 1);
		return;
	}

	vec4 t = texture(textureSampler, UV);

	if (t.x == 0.0 && t.y == 0.0 && t.z == 0.0)
	{
		color = vec4(0, 0, 0, 1);
	}
	else
	{
		color = inColor;
	}
};