#version 130

uniform int levelSize;
uniform float objectID;
uniform vec2 nearFar;
uniform bool bendingActivated;

void mainDeform();

out vec2 uv;
out vec3 vertex_objCoords;
out vec4 bendedVertex;

void main()
{

	uv = gl_Vertex.xy / levelSize + vec2(0.5, 0.5);

	vertex_objCoords = gl_Vertex.xyz;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex ;

	if (bendingActivated)
		mainDeform();

	bendedVertex = gl_Position;

	return;
}