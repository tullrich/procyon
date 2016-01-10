#version 130

uniform sampler2D tex;

in vec2 uvcoords;
in vec3 tint;

out vec4 outColor;

void main()
{
	outColor = texture( tex, uvcoords ) + vec4( tint, 0.0f );
}
