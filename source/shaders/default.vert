#version 130

uniform float objectID;
uniform float[64] frequencies;
out vec2 uv;

out float scaled_height;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void)
{
	uv =  gl_MultiTexCoord0.xy;

	if (gl_Vertex.z > 5.0) {
		// highp int index = clamp(0, 63, int( mod(gl_Vertex.y * 10, 63) ));
		highp int index = int(smoothstep(0, 5000, gl_Vertex.y) * 63);



		float randomFactor = 100 * rand(mod(gl_Vertex.xy, 250));
		// currently, only frequencies[0] seems to have values?
		gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + vec4(0, 0, frequencies[index] * randomFactor, 0) );
	}
	else
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	scaled_height = gl_Vertex.z;
}