#version 130

uniform sampler2D reflectionTex;
uniform samplerCube skyDome;
uniform int modelID;
uniform vec2 nearFar;
uniform float glowIntensity;

in vec2 uv;
in vec3 vertex_objCoords;
//in vec3 v_normal;


void main()
{
	int inverseFrequency = 80;
	
	float modifier = 2;
	float smoothingFactorX = abs(mod((5000 / modifier * uv.x), inverseFrequency / modifier) - 5);
	float smoothingFactorY = abs(mod((5000 / modifier * uv.y), inverseFrequency / modifier) - 5);
	
	vec4 grid  = mix(
		vec4(0.086, 0.45, 0.513, 1.0),
		vec4(0.0, 0.0, 0.0, 1),
		clamp(0.0, 1.0, pow(min(smoothingFactorX, smoothingFactorY), 0.5))
	); // sceneColor
	//vec3 r = normalize(reflect(v_eye,normalize(v_normal)));
	//vec3 q = refract(e,n,1/1.03);
	
	//float frsl = -1 * dot(e, n) ;

	//vec4 refl = env(r);
	//vec4 refr = env(q);
	
	//gl_FragData[0] = textureCube(skyDome,r);
	
	//clipReflection height = 10.0 because ground is at -10 and is flipped;)
	vec4 vClipReflection = gl_ModelViewProjectionMatrix * vec4(vertex_objCoords.xy, 10.0 , 1.0);
	vec2 vDeviceReflection = vClipReflection.xy / vClipReflection.w;
	vec2 vTextureReflection = vec2(0.5) + 0.5 * vDeviceReflection;

	vec4 reflectionTextureColor = texture(reflectionTex, vTextureReflection);
	
	//vec3 r = normalize(reflect(e,n));
	
	gl_FragData[0] = vec4(mix(grid * 2.0, reflectionTextureColor, 0.8));//texture(reflectionTex,uv);
	int glowIntesity_int = int(clamp(0.0,100.0,glowIntensity*100.0));
	//8bit int, 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, glowIntensity,0,0);
}