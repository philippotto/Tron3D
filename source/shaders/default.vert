#version 130

uniform float objectID;
out vec2 uv;

void oculusDeform();
out float scaled_height;

void main(void)
{
	uv =  gl_MultiTexCoord0.xy;
	scaled_height = gl_Vertex.z;
	gl_Position = gl_ModelViewProjectionMatrix  * gl_Vertex;
	oculusDeform();
}