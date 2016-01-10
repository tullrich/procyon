#version 130

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform float triangleColor;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
	vec4 sampleone = texture( tex, Texcoord ) * triangleColor;
	vec4 sampletwo = texture( tex2, Texcoord ) * ( 1.0f - triangleColor );
	outColor = sampleone + sampletwo;
}