#version 130

in vec2 normal;

uniform vec4 color;
uniform float feather;

out vec4 outColor;

void main()
{
	float width = length( normal );
	float alpha = clamp( mix( 0.0, 1.0, (1.0-width) / feather ), 0.0, 1.0 );
	outColor = vec4( color.xyz, color.w * alpha );
}
