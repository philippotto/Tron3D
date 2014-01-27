#version 130

uniform sampler2D sceneLayer;
uniform sampler2D idLayer;

void main(void)
{
	vec2 st = gl_TexCoord[0].st;
	float objectID = texture2D(idLayer, st).x;

	if (objectID != 1.f){
		gl_FragColor = vec4(0.f, 0.f, 0.f, 0.f);
		return;
	}

	vec4 sceneColor = texture2D(sceneLayer, st);
	gl_FragColor = sceneColor;
}