uniform samplerCube cubemap;

void main() {
    gl_FragColor = texture(cubemap, gl_TexCoord[0].xyz);
}