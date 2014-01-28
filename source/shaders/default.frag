#version 130
uniform sampler2D diffuseTexture;
uniform int modelID;
uniform float glowIntensity;

in vec2 uv;
in float scaled_height;

void main() {
	vec4 default_tex =  texture(diffuseTexture, uv);
	gl_FragData[0] = vec4(default_tex.x,default_tex.x / 2.0, default_tex.x, 1.0);;
	//vec4(1.0 - scaled_height / 50.0);
	//3channels: select_group, attribute (f.e glowintensity for glow group), height in world coords
	gl_FragData[1] = vec4(modelID, glowIntensity,  scaled_height / 200.0, 0);
}