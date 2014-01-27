#version 130

uniform float objectID;
uniform vec2 nearFar;

void main(void)
{
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	gl_Position = ftransform();

	return;
}