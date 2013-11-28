uniform samplerCube cubemap;

void main() {
    gl_FragData[0] = texture(cubemap, gl_TexCoord[0].xyz);
}