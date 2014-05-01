#version 130
uniform sampler2D diffuseTexture;
uniform int modelID;
uniform float glowIntensity;
uniform float trueColor;
uniform float alpha;

in vec2 uv;
in float scaled_height;

void main() {
	vec4 diffuseColor =  vec4(texture(diffuseTexture, uv).xyz, alpha);
	vec4 adjustedColor = vec4(diffuseColor.x, diffuseColor.x / 2.0, diffuseColor.x, 1.0);

	// decide whether to use the original or adjusted color, based on the trueColor uniform
	gl_FragData[0] = mix(adjustedColor, diffuseColor, trueColor);

	/*MR: for some very akward reason, the last parameter has to be set to 1 here (although the texture is only GL_RGB..), to render the ramps
	and blocks into the idlayer, while it does not affect other objects like the itemboxes which also use default.frag..*/
	gl_FragData[1] = vec4(modelID, glowIntensity, 0,1);
}