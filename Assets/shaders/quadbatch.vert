#version 130

in vec2 	vertPosition;
#ifdef UV0_ENABLED
in vec2 	uv;
#endif

in vec2 	quadPos;
in vec2 	quadSize;
in float 	quadRotRads;
in vec4		quadTint;
#ifdef UV0_ENABLED
in vec2 	quadUVOffset;
in vec2 	quadUVSize;
#endif

#ifdef UV0_ENABLED
out vec2 	uvcoords;
#endif
out vec4 	color;

uniform mat3 screenSpaceTransform;

void main()
{
	color = quadTint;
#ifdef UV0_ENABLED
	uvcoords = uv * quadUVSize + quadUVOffset;
#endif

	float sinRot = sin(quadRotRads);
	float cosRot = cos(quadRotRads);
	mat2 rotMat = mat2( cosRot, sinRot, -sinRot, cosRot );
	vec2 worldPos = ( rotMat * vertPosition ) * quadSize + quadPos;
	gl_Position = vec4( ( screenSpaceTransform * vec3( worldPos, 1.0f ) ).xy, 0.0f, 1.0f );
}
