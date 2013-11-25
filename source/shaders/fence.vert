#version 130

void main()
{                              
	gl_Position = gl_ModelViewProjectionMatrix  * gl_Vertex ;
}
