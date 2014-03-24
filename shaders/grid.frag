#version 130

uniform sampler2D reflectionTex;
//uniform sampler2D reflectionAttrib;
uniform samplerCube skyDome;
uniform sampler2D diffuseTexture;


in vec2 uv;

uniform int modelID;
uniform vec2 nearFar;
uniform float glowIntensity;
uniform vec3 cameraEye;
uniform float bendingFactor;

in vec3 vertex_objCoords;
in vec4 bendedVertex;

const vec3 v_normal = vec3(0.0,0.0,1.0);

void main()
{
	float grid_stripe =  texture(diffuseTexture, 45 * uv).x;
	vec4 grid = vec4(0.0, grid_stripe / 2.0, grid_stripe, 1.0);


	vec3 shift = vec3(0.0, 0.0, -10.0);
	vec3 v_eye = (vertex_objCoords + shift - cameraEye).xyz;
	vec3 r = normalize(reflect(normalize(v_eye), v_normal));
	vec4 skydome_reflection = textureCube(skyDome, r);


	//un-deform reflection
	//clipReflection height = 10.0 because ground is at -10 and is flipped;)
	vec4 vClipReflection = bendedVertex;//gl_ModelViewProjectionMatrix * vec4(vertex_objCoords.xy, 0.0 , 1.0);
	vec2 vDeviceReflection = vClipReflection.xy / vClipReflection.w;
	vec2 vTextureReflection = vec2(0.5) + 0.5 * vDeviceReflection;
	// vec4 reflected_height = vec4(texture(reflectionAttrib,vTextureReflection).b);
	vec4 scene_reflection = texture(reflectionTex, vTextureReflection); //mix(texture(reflectionTex, vTextureReflection), vec4(0.0), reflected_height);

	float xy_dist = length(v_eye) / 330.0;
	float skybox_blend = clamp(smoothstep(0.0, 0.2, xy_dist) * xy_dist, 0.0, 0.6);
	float bending_blend = step(0.01,bendingFactor) * clamp(smoothstep(0.0, 0.15, xy_dist  * xy_dist), 0.0, 0.9);
	//occlude skybox, when reflection occludes it
	float occlusion_fac = (1.0 - scene_reflection.a);
	//dont use skybox in a parameter around player
	vec4 reflectionTextureColor = mix(scene_reflection, skydome_reflection, skybox_blend * occlusion_fac + bending_blend);
	
	// reflectionTextureColor = mix(reflectionTextureColor,vec4(0.0),  xy_dist);
	//blend farther away grid to black if bending
	reflectionTextureColor = mix(reflectionTextureColor, vec4(0.0), bending_blend);
	//vec4(skybox_blend,0.0,1.0);//
	gl_FragData[0] = mix(grid * 2.0, reflectionTextureColor, 0.5 - (1 - bendingFactor) / 4);
	// gl_FragData[0] = vec4(bendingFactor);
	// 2 channels: select_group, attribute (f.e glowintensity for glow group
	gl_FragData[1] = vec4(modelID, glowIntensity, 0, 0);

	return;
}