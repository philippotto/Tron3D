#version 130


uniform sampler2D reflectionTex;
uniform samplerCube skyDome;
uniform sampler2D diffuseTexture;

in vec2 uv;
uniform int modelID;
uniform vec2 nearFar;
uniform float glowIntensity;

in vec3 vertex_objCoords;
in vec3 v_normal;
in vec3 v_eye;


void main()
{	
	float grid_stripe =  texture(diffuseTexture, 45 * uv).x;
	vec4 grid = vec4(0.0,grid_stripe / 2.0, grid_stripe, 1.0);

	//vec3 r = normalize(reflect(v_eye, v_normal));
	//vec4 skydome_reflection = textureCube(skyDome, r);
	
	//float frsl = -1 * dot(e, n) ;

	//vec4 refl = env(r);
	//vec4 refr = env(q);
	
	//gl_FragData[0] = textureCube(skyDome,r);
	
	//un-deform reflection
	//clipReflection height = 10.0 because ground is at -10 and is flipped;)
	vec4 vClipReflection = gl_ModelViewProjectionMatrix * vec4(vertex_objCoords.xy, 10.0 , 1.0);
	vec2 vDeviceReflection = vClipReflection.xy / vClipReflection.w;
	vec2 vTextureReflection = vec2(0.5) + 0.5 * vDeviceReflection;

	vec4 reflectionTextureColor = texture(reflectionTex, vTextureReflection);//mix(texture(reflectionTex, vTextureReflection), vec4(1.0),0.2);
	
	//vec3 r = normalize(reflect(e,n));
	gl_FragData[0] = mix(grid * 2.0, reflectionTextureColor, 0.8);

	// 2 channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID, glowIntensity,0,0);
}