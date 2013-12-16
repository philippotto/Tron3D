uniform float objectID;
uniform vec2 nearFar;
varying vec3 theNormal;
varying float linearDepth;

void main(void)
{
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_TexCoord[1] = vec4(objectID);

	gl_Position = ftransform();
	theNormal = normalize(gl_NormalMatrix * gl_Normal);
}