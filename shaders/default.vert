#version 130

uniform float objectID;
uniform bool isReflecting;
out vec2 uv;
uniform bool bendingActivated;

void mainDeform();
void mainDeformReflected();

out float scaled_height;

void main(void)
{
	uv =  gl_MultiTexCoord0.xy;
	scaled_height = gl_Vertex.z;


	if (isReflecting || !bendingActivated) {
		gl_Position = gl_ModelViewProjectionMatrix  * gl_Vertex;
		//TODO: make mainDeformReflected work
		//mainDeformReflected();
	} else {
		mainDeform();
	}
	return;

}