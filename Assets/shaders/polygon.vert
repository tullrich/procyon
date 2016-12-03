#version 130

in vec2 vertPosition;
in vec4 vertColor;

uniform mat3 u_mv_matrix;
uniform mat3 u_p_matrix;

out vec4 color;

void main()
{
	vec3 clip = u_p_matrix * u_mv_matrix * vec3( vertPosition, 1.0 );
	gl_Position = vec4( clip.xy, 0.0f, 1.0 );
	color = vertColor;
}
