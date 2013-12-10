uniform sampler2D colorTex;
uniform float objectID;


varying vec3 theNormal;
varying float linearDepth;

vec4 getPhongShadedSceneColor()
{
	// get normalized normal
	vec3 N = normalize(theNormal);

	// get the light's direction - since we have a directional light, the position already is the direction
	vec3 L = normalize(vec3(gl_LightSource[0].position.xyz));

	// get the half vector between light direction and viewing direction (camera position is assumed to be infinitely far away)
	vec3 H = normalize(gl_LightSource[0].halfVector.xyz);

	// compute terms for each kind of light from lightsources and materials
	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;	
	vec4 specular = clamp(gl_FrontMaterial.specular * gl_LightSource[0].specular, vec4(0.0), vec4(1.0));

	// dot products for computation of diffuse and specular light
	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);

	// set final color
	return ambient + diffuse * NdotL;// + specular * pow(NdotH, gl_FrontMaterial.shininess);
}

void main(void)
{		
    // has texture? 
    vec4 texColor = vec4(1.0);

    const bool hasTexture = false;
    if (hasTexture)
    {
        texColor = texture2D(colorTex, gl_TexCoord[0].st);
    }
	gl_FragData[0] = texColor * getPhongShadedSceneColor(); // sceneColor

	gl_FragData[1] = vec4(normalize(theNormal), linearDepth); // normal and depth
	
    // change this to use other attributes for the object id, e.g., texcoords
    gl_FragData[2] = vec4(objectID,0,0,0); // id
}