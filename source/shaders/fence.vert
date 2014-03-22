#version 130

in float a_relHeight;
out float v_relHeight;
uniform bool bendingActivated;
uniform bool isReflecting;

void mainDeform();

void main()
{
	// this is the relative height of the fence between 0 and 1
	v_relHeight = a_relHeight;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	if (false && bendingActivated && !isReflecting)
		mainDeform();
	return;
}