uniform sampler2D colorTex;
uniform float objectID;


varying vec3 theNormal;
varying float linearDepth;

void main(void)
{		
    // has texture? 
    vec4 texColor = vec4(0.0);

    const bool hasTexture = false;
    if (hasTexture)
    {
        texColor = texture2D(colorTex, gl_TexCoord[0].st);
    }
	gl_FragData[0] = texColor; // sceneColor

	gl_FragData[1] = vec4(normalize(theNormal), linearDepth); // normal and depth
	
    // change this to use other attributes for the object id, e.g., texcoords
    gl_FragData[2] = vec4(objectID,0,0,0); // id
}