#version 130

uniform mat4 occViewAdjust;
uniform mat4 occProjection;


void oculusDeform()
{


	// gl_ProjectionMatrix
	// gl_ModelViewProjectionMatrix
	//


	mat4 modelView =  gl_ModelViewMatrix; // inverse(gl_ProjectionMatrix) * gl_ModelViewProjectionMatrix;


	mat4 moddedProj = occProjection;
	// moddedProj[2][2] = 1.f;
	// moddedProj[3][3] = 1.f;
	gl_Position = occProjection * gl_ProjectionMatrix * (occViewAdjust * (modelView * gl_Vertex));

	// gl_Position = vec4(1.f);

	// associative
	// m1 * m2 * v === (m1 * m2) * v == m1 * (m2 * v)

	// occProjection seems to hold:
	// ~
	// 1       0        0      0
	// 0       1        0      0
	// 0       0        0      0
	// 0       0        0      0



	// gl_Position = vec4(0.f);
}

// void main() {
// 	oculusDeform();
// }