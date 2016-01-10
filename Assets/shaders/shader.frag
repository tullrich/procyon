#version 130

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
	outColor = texture( tex, Texcoord );
}