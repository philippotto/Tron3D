#version 130

uniform sampler2D diffuseTexture;
in vec2 uv;
uniform int modelID;
uniform float glowIntensity;

void main() {
	gl_FragData[0] = vec4(0.0, texture(diffuseTexture, uv).x * 1.5, texture(diffuseTexture, uv).x * 1.5, 1.0);

	//8bit int, 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, glowIntensity, 0, 0);
}