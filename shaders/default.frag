#version 130
uniform sampler2D diffuseTexture;
uniform int modelID;
uniform float glowIntensity;
uniform float trueColor;
uniform float alpha;

in vec2 uv;
in float scaled_height;

void main() {
	vec4 diffuseColor =  vec4(texture(diffuseTexture, uv).xyz,alpha);
	vec4 adjustedColor = vec4(diffuseColor.x, diffuseColor.x / 2.0, diffuseColor.x, 1.0);

	// decide whether to use the original or adjusted color, based on the trueColor uniform
	gl_FragData[0] = mix(adjustedColor, diffuseColor, trueColor);

	//3channels: select_group, attribute (f.e glowintensity for glow group), height in world coords
	gl_FragData[1] = vec4(modelID, glowIntensity, scaled_height / 200.0, 0);
}