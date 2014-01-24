#version 130

uniform int levelSize;
uniform float objectID;
uniform vec2 nearFar;
uniform vec3 cameraEye;

out vec2 uv;
out vec3 vertex_objCoords;
//out vec3 v_normal;
void main()
{
	uv = (gl_Vertex).xy / levelSize + vec2(0.5, 0.5);
	//v_eye = normalize((gl_ModelViewMatrix *  gl_Vertex).xyz - cameraEye);
	
	vertex_objCoords = gl_Vertex;
	gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex ;
	
	// theNormal = normalize(gl_NormalMatrix * gl_Normal);
	// linearDepth = (-(gl_ModelViewMatrix * gl_Vertex).z-nearFar.x)/(nearFar.y-nearFar.x);
	// gl_TexCoord[1] = vec4(objectID);

}