#version 130

uniform vec3 fenceColor;
uniform float fenceHeight;

in vec3 vertex;

void main()
{
	float mixFactor = (fenceHeight / 2.0 - abs(vertex.z - fenceHeight / 2.0)) / 3.0;

	vec4 color = mix(
		vec4(fenceColor, 1.0),
		vec4(fenceColor, 0.10),
		min(mixFactor, 1.0)
	);

	gl_FragColor = vec4(color);
}