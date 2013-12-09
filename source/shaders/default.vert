#version 130

uniform vec4 diffuseMaterialColor;
uniform vec4 ambientMaterialColor;
uniform mat4 transform;
uniform mat4 osg_ViewMatrixInverse;
uniform vec3 playerColor;

out vec2 texCoord;

out vec3 diffuseColor; 
            // the diffuse Phong lighting computed in the vertex shader

out vec3 normalDirection;
out vec3 viewDirection;
out float attenuation;
out vec3 lightDirection;

const vec4 lightPosition = vec4(0.0,-50.0,0.0,1.0) ;
const vec3 diffuseLight = vec3(1.0,1.0,1.0);

struct lightSource
{
  vec4 position;
  vec4 diffuse;
  vec4 specular;
  float constantAttenuation, linearAttenuation, quadraticAttenuation;
  float spotCutoff, spotExponent;
  vec3 spotDirection;
};

lightSource light0 = lightSource(
  vec4(0.0,  10.0,  1.0, 1.0),
  vec4(1.0,  1.0,  1.0, 1.0),
  vec4(1.0,  1.0,  1.0, 1.0),
  0.0, 1.0, 0.0,
  180.0, 0.0,
  vec3(0.0, 0.0, 0.0)
);

void main()
{                              
	
	normalDirection = normalize(gl_NormalMatrix * gl_Normal);
	viewDirection = -normalize(vec3(gl_ModelViewMatrix * gl_Vertex)); 
	// viewDirection = normalize(osg_ViewMatrixInverse* vec4(0.0,0.0,0.0,1.0) 
	//	 -  osg_ViewMatrixInverse*gl_ModelViewMatrix * gl_Vertex);
	
	vec3 vertexToLightSource =   vec4(light0.position
	-  gl_ModelViewMatrix *  gl_Vertex).xyz;
	
	
	float distance = length(light0.position);
	
	attenuation = 1.0;
	
	lightDirection = normalize(vertexToLightSource);

	vec3 ambientLighting = ambientMaterialColor.xyz * gl_LightModel.ambient.xyz  ; 
	   // without material color!

	vec3 diffuseReflection = attenuation 
	   * diffuseLight.xyz
	   * max(0.0, dot(normalDirection, lightDirection))
	   * diffuseMaterialColor.xyz;
	  // diffuseReflection = diffuseMaterialColor.xyz;
	   // without material color!

	diffuseColor = (ambientLighting + diffuseReflection) * vec3(1.f, 1.f, 0.f);

	texCoord = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix   * gl_Vertex ;
}
