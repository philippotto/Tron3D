uniform float objectID;
out vec2 uv;

void main(void)
{
	uv =  gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix  * gl_Vertex  ;
}