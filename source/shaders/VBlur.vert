#version 130

out vec2 v_texCoord;
out vec2 v_blurTexCoords[14];
out float doHorizontalBlur;

void main()
{
	// todo: TexCoords nutzen?
	gl_Position = gl_Vertex;
	v_texCoord = (gl_Vertex.xy + vec2(1.0)) / 2.0;
	doHorizontalBlur = 0.f;

	return;
}