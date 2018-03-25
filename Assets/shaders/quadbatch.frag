#version 130

uniform sampler2D tex;

in vec2 uvcoords;
in vec3 tint;

out vec4 outColor;

void main()
{
	vec2 texDims = vec2( textureSize( tex, 0 ) );
	outColor = texture( tex, uvcoords / texDims ) + vec4( tint, 0.0f );
}
