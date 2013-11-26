#version 130

out vec3 vertex;

void main()
{
	vertex = gl_Vertex.xyz;
	gl_Position = gl_ModelViewProjectionMatrix  * gl_Vertex ;
}