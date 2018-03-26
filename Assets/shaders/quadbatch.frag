#version 130

#ifdef TEXTURE0_ENABLED
uniform sampler2D tex;
#endif

#ifdef UV0_ENABLED
in vec2 uvcoords;
#endif
in vec4 color;

out vec4 outColor;

void main()
{
	vec4 finalColor = color;
#ifdef TEXTURE0_ENABLED
	vec2 texDims = vec2( textureSize( tex, 0 ) );
	finalColor *= texture( tex, uvcoords / texDims );
#endif
	outColor = finalColor;
}
