#version 130

uniform sampler2D diffuseTexture;
uniform int modelID;
in vec2 uv;

void main() {
	gl_FragData[0] = texture(diffuseTexture, uv);

	gl_FragData[1] = vec4(1.0f); // normal and depth
	gl_FragData[2] = vec4(modelID, 1.f, 0, 0); // id
}