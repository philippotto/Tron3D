#version 130

uniform float objectID;
out vec2 uv;


void mainDeform();

void main(void)
{
	uv =  gl_MultiTexCoord0.xy;
	// gl_Position = gl_ModelViewProjectionMatrix  * gl_Vertex  ;

	mainDeform();
	return;
}