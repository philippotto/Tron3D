#version 130

uniform sampler2D colorTex;
in vec2 uv;
uniform int modelID;
uniform float glowIntensity;
uniform int levelSize;

in vec3 theNormal;
in float linearDepth;

void main()
{
	int inverseFrequency = 100;
	
	float modifier = 2;
	float smoothingFactorX = abs(mod((5000 / modifier * uv.x), inverseFrequency / modifier) - 4.8);
	//some magic numbers, adjusted to the wall height
	float smoothingFactorY = abs(mod((4300 / modifier * uv.y), inverseFrequency / 16.0) -0.7);
	gl_FragData[0] = mix(
		vec4(22, 115, 131, 100) / 255.f,
		vec4(1, 1, 1, 0) / 255.f,
		pow(min(smoothingFactorX, smoothingFactorY), 0.5));

	//8bit int, 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, glowIntensity,0,0);
}