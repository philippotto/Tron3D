#version 130
uniform sampler2D diffuseTexture;
in vec2 uv;
uniform int modelID;
uniform float glowIntensity;

void main() {
	vec4 default_tex =  texture(diffuseTexture, uv);
	gl_FragData[0] = vec4(default_tex.x,default_tex.x / 2.0, default_tex.x, 1.0);;
	
	//8bit int, 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, glowIntensity, 0, 0);
}