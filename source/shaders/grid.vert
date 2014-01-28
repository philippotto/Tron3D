#version 130

uniform int levelSize;
uniform float objectID;
uniform vec2 nearFar;

uniform vec3 cameraEye;
uniform mat4 cameraViewMatrix;

out vec2 uv;
out vec3 vertex_objCoords;
out vec3 v_normal;
out vec3 v_eye;

void main()
{
	uv = gl_Vertex.xy / levelSize + vec2(0.5, 0.5);
	//v_eye = normalize((gl_ModelViewMatrix *  gl_Vertex).xyz - cameraEye);

	// vec4 worldPosition = gl_Vertex;
    // vec4 viewPosition = gl_Vertex * cameraViewMatrix;

    v_eye = normalize(gl_Vertex.xyz - cameraEye);
 
    // vec3 normal = normalize(input.Normal * WorldInverseTranspose);
    // output.Reflection = reflect(-normalize(ViewDirection), normalize(Normal));
	
	vertex_objCoords = gl_Vertex.xyz;
	gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
	v_normal = vec3(0.0,0.0,1.0);

	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex ;
}