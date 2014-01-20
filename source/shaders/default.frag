#version 130
uniform sampler2D diffuseTexture;
uniform int modelID;
in vec2 uv;

void main() {
	gl_FragData[0] = texture(diffuseTexture, uv);
	
	//8bit int, 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, 100,0,0);
}