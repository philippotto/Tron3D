uniform samplerCube cubemap;
uniform int modelID;

void main() {
  gl_FragData[0] = texture(cubemap, gl_TexCoord[0].xyz);

	//8bit int, 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, 0,0,0);
}