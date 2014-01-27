#version 130

uniform sampler2D diffuseTexture;
uniform int modelID;
uniform float glowIntensity;
in vec2 uv;

void main() {
	gl_FragData[0] = vec4(texture(diffuseTexture, uv).x, 0.0, 0.0, 1.0);
	
	//8bit int, 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, glowIntensity, 0, 0);
}