#version 130

out vec2 texCoord;

out vec3 diffuseColor; 
            // the diffuse Phong lighting computed in the vertex shader

out vec3 normalDirection;
out vec3 viewDirection;
out float attenuation;
out vec3 lightDirection;

void main()
{                              
	lightDirection = vec3(0.0,-1.0,0.0);
	vec4 lightPosition = vec4(1.0,10.0,0.0,0.0) ;
	vec3 diffuseLight = vec3(1.0,1.0,1.0);
	
	
	normalDirection = normalize(gl_NormalMatrix * gl_Normal);
	viewDirection = -normalize(vec3(gl_ModelViewMatrix * gl_Vertex)); 
	
	   vec3 vertexToLightSource = 
		  vec3(lightPosition
		  - gl_ModelViewMatrix * gl_Vertex);
	   float distance = length(vertexToLightSource);
	   attenuation = 1.0 ;/// distance; // linear attenuation 
	   lightDirection = normalize(vertexToLightSource);

	vec3 ambientLighting = vec3(gl_LightModel.ambient); 
	   // without material color!

	vec3 diffuseReflection = attenuation 
	   * vec3(diffuseLight) 
	   * max(0.0, dot(normalDirection, lightDirection))
	   * gl_FrontMaterial.diffuse.xyz;
	   // without material color!

	diffuseColor = ambientLighting + diffuseReflection;

	texCoord = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix   * gl_Vertex ;
}
