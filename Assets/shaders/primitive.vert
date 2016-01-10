#version 130

in vec2 	vertPosition;

uniform mat3 projectionMat;
uniform mat3 modelViewMat;

void main()
{
	vec3 clip = projectionMat * modelViewMat * vec3( vertPosition, 1.0 );
	gl_Position = vec4( clip.xy, 0.0f, 1.0 );
}