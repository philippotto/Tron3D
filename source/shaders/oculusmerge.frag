#version 130

uniform sampler2D left;
uniform sampler2D right;


//////////////////////////////////////////////////////////////////////////
// Final Color Composition
//////////////////////////////////////////////////////////////////////////

void main(void)
{
	vec2 st = gl_TexCoord[0].st;

	vec4 leftColor = texture2D(left, st);
	vec4 rightColor = texture2D(right, st);

	// gl_FragColor = vec4(1.f);
	gl_FragColor = leftColor;
}