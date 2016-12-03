#version 130

in vec2 vertPosition;

uniform mat3 projectionMat;
uniform mat3 modelViewMat;

void main()
{
	vec3 world = modelViewMat * vec3( vertPosition, 1.0 );
	vec3 clip = projectionMat * world;
	gl_Position = vec4( clip.x, clip.y, 0.0, 1.0 );
}
