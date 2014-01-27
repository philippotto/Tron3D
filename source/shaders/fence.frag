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
	int glowIntesity_int = int(clamp(0.0,100.0,glowIntensity*100.0));
	//8bit int, 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, glowIntensity, 0, 0);

	return;
}