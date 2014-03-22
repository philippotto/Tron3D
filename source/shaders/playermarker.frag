#version 130
uniform int modelID;
uniform float glowIntensity;
uniform vec4 playerColor;

in vec2 uv;
in float scaled_height;

void main() {
	gl_FragData[0] = playerColor;

	//3channels: select_group, attribute (f.e glowintensity for glow group), height in world coords
	gl_FragData[1] = vec4(modelID, glowIntensity, 0, 0);
}