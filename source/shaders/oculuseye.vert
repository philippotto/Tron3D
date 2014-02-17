#version 130

uniform mat4 occViewAdjust;
uniform mat4 occProjection;


void oculusDeform()
{
	mat4 modelView =  gl_ModelViewMatrix;
	mat4 moddedProj = occProjection;
	gl_Position = occProjection * gl_ProjectionMatrix * (occViewAdjust * (modelView * gl_Vertex));
}