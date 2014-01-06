#version 130

uniform vec3 fenceColor;
uniform int modelID;
uniform float glowIntensity;

in float v_relHeight;

void main()
{
	float mixFactor = (0.5 - abs(v_relHeight - 0.5)) / 0.5;

	vec4 color = mix(
		vec4(fenceColor, 1.0),
		vec4(fenceColor, 0.10),
		mixFactor
	);

	gl_FragData[0] = color;
	gl_FragData[1] = vec4(0.5f); // normal and depth
	gl_FragData[2] = vec4(modelID, glowIntensity, 0, 0);
}