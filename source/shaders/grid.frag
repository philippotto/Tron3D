#version 130

uniform sampler2D reflectionTex;
uniform samplerCube skyDome;
in vec2 uv;
uniform int modelID;
uniform float glowIntensity;

void main()
{
	int inverseFrequency = 100;
	
	float modifier = 2;
	float smoothingFactorX = abs(mod((5000 / modifier * uv.x), inverseFrequency / modifier) - 5);
	float smoothingFactorY = abs(mod((5000 / modifier * uv.y), inverseFrequency / modifier) - 5);
	
	// gl_FragData[0] = mix(
		// vec4(22, 115, 131, 1) / 255.f,
		// vec4(0, 0, 0, 1) / 255.f,
		// pow(min(smoothingFactorX, smoothingFactorY), 0.5)
	// ); // sceneColor

	gl_FragData[0] = texture(skyDome,uv.xyx);
	
	int glowIntesity_int = int(clamp(0.0,100.0,glowIntensity*100.0));
	//8bit int, 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, glowIntensity,0,0);
}