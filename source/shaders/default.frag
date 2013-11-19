#version 140
uniform sampler2D specularTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

in vec2 texCoord;

in vec3 normalDirection;
in vec3 viewDirection;
in vec3 diffuseColor;

in vec3 lightDirection;

in float attenuation;

void main()
{
	float shininess = 10.0;

	vec3 specularReflection;
	
	vec3 normal = normalDirection *   normalize(texture2D(normalTexture, texCoord).rgb * 2.0 - 1.0);

	   // light source on the wrong side?
	if (dot(normal, lightDirection) < 0.0) 
	{
	  // no specular reflection
	   specularReflection = vec3(0.0, 0.0, 0.0); 
	}
	else // light source on the right side
	{
		  specularReflection = attenuation 
                  * vec3(1.1) 
                  * vec3(gl_FrontMaterial.specular)
				  * texture(specularTexture,texCoord).rgb
                  * pow(max(0.0, dot(reflect(-lightDirection, 
                  normal), viewDirection)), 
                  gl_FrontMaterial.shininess);
	}
	
	//vec3 diffuse = max(dot(normal, 
	
	gl_FragColor = vec4(diffuseColor  * texture2D(diffuseTexture,texCoord).rgb + specularReflection, texture2D(diffuseTexture,texCoord).a);
}
