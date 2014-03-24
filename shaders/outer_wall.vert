#version 130

uniform int levelSize;
uniform float objectID;
uniform vec2 nearFar;
out vec3 theNormal;
out float linearDepth;

out vec2 uv;

void mainDeform();

void main()
{
	theNormal = normalize(gl_NormalMatrix * gl_Normal);
	//subtract 1 depending on normal facing in the arena, fixes a stripe shift issue on the walls
	float x_component = gl_Vertex.x - step(0.5, gl_Normal.x + gl_Normal.y + gl_Normal.z);
	uv = vec2((x_component + gl_Vertex.y), gl_Vertex.z) / levelSize + vec2(0.5, 0.5);

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex  ;

	linearDepth = (-(gl_ModelViewMatrix * gl_Vertex).z - nearFar.x) / (nearFar.y - nearFar.x);
	gl_TexCoord[1] = vec4(objectID);
	mainDeform();

	return;
}