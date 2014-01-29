#version 130

uniform float objectID;
uniform float[64] frequencies;
out vec2 uv;

void main(void)
{
	uv =  gl_MultiTexCoord0.xy;
	if (gl_Vertex.z > 0.0) {
		// highp int index = clamp(0, 63, int( mod(gl_Vertex.y * 10, 63) ));
		// currently, only frequencies[0] seems to have values?
		gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + vec4(0, 0, frequencies[0] * 10, 0) );
	}
	else
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}