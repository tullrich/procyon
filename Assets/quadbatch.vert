#version 130

in vec2 	vertPosition;
in vec2 	uv;

in vec2 	quadPos;
in vec2 	quadSize;
in float 	quadRotRads;
in vec2 	quadUVOffset;
in vec2 	quadUVSize;
in vec3		quadTint;

out vec2 	uvcoords;
out vec3 	tint;

uniform mat3 screenSpaceTransform;

void main()
{
	tint = quadTint;
	uvcoords = uv * quadUVSize + quadUVOffset;

	float sinRot = sin(quadRotRads);
	float cosRot = cos(quadRotRads);
	mat2 rotMat = mat2( cosRot, sinRot, -sinRot, cosRot );

	vec2 worldPos = ( rotMat * vertPosition ) * quadSize + quadPos;

	gl_Position = vec4( ( screenSpaceTransform * vec3( worldPos, 1.0 ) ).xy, 0.0f, 1.0 );
}