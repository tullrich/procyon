#version 130

in vec2 position;

uniform vec4 color;
uniform mat3 mvp;

void main()
{
	gl_Position = vec4( ( mvp * vec3( position, 1.0 ) ).xy, 0.0f, 1.0 );
}