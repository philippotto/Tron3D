uniform samplerCube cubemap;
uniform int modelID;

void main() {
  gl_FragData[0] = texture(cubemap, gl_TexCoord[0].xyz);

	gl_FragData[1] = vec4(0.5f); // normal and depth
  gl_FragData[2] = vec4(modelID, 1.f, 0, 0); // id
}