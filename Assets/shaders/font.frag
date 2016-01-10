#version 130

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;

void main()
{
	outColor = vec4( 1.0f, 0.0f, 0.0f, texture( tex, Texcoord ).w);
}