#version 120

uniform sampler2D sceneLayer;
uniform sampler2D idLayer;

uniform sampler2D pongLayer;

//////////////////////////////////////////////////////////////////////////
// Final Color Composition
//////////////////////////////////////////////////////////////////////////
void main(void)
{
	vec2 st = gl_TexCoord[0].st;

	vec4 sceneColor = texture2D(sceneLayer, st);
	vec4 pongColor = texture2D(pongLayer, st);

    gl_FragColor = pongColor * 3 + sceneColor;
}