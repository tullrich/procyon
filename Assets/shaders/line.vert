#version 130

in vec2 vertPosition;
in vec2 vertNormal;

uniform float u_linewidth;
uniform mat3 u_mv_matrix;
uniform mat3 u_p_matrix;

out vec2 normal;

void main() {
	normal = vertNormal;
    vec3 delta = vec3(vertNormal * u_linewidth, 0.0 );
    vec3 pos = u_mv_matrix * vec3(vertPosition, 1.0 );
    gl_Position = vec4( ( u_p_matrix * ( pos + delta ) ).xy, 0.0, 1.0 );
}
