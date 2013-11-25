#version 130

uniform vec3 fenceColor;

void main()
{
	gl_FragColor = vec4(fenceColor, 1.0);
}
