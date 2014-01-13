#version 130

uniform sampler2D sceneLayer;
uniform sampler2D idLayer;
uniform sampler2D pongLayer;
uniform sampler2D oldLayer;

//////////////////////////////////////////////////////////////////////////
// Final Color Composition
//////////////////////////////////////////////////////////////////////////
void main(void)
{
	vec2 st = gl_TexCoord[0].st;

	vec4 sceneColor = texture2D(sceneLayer, st);
	vec4 pongColor = texture2D(pongLayer, st);
	vec4 oldColor = texture2D(oldLayer, st);

	float oldFrameWeight = 0.8;
	float newFrameWeight = 1 - oldFrameWeight;
	gl_FragColor = newFrameWeight * (pongColor * 3 + sceneColor) + oldFrameWeight * oldColor;
}