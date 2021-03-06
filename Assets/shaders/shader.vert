#version 130

in vec2 position;
in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec2 Texcoord;

uniform mat3 uv_mvp;
uniform mat3 mvp;

void main()
{
	Color = color;
    Texcoord = ( uv_mvp * vec3( texcoord, 1.0 ) ).xy;
	gl_Position = vec4( ( mvp * vec3( position, 1.0 ) ).xy, 0.0f, 1.0 );
}
