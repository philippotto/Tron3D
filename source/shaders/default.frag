#version 140
uniform sampler2D specularTexture;
uniform sampler2D diffuseTexture;

in vec2 texCoord;
void main()
{
	gl_FragColor = texture2D(specularTexture,texCoord);//vec4(1.0,0.0,0.0, 1.0);
}
