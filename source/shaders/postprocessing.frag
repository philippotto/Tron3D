#version 130

uniform sampler2D sceneLayer;
uniform sampler2D normalDepthLayer;
uniform sampler2D idLayer;

uniform sampler2D voronoiLayer;

//////////////////////////////////////////////////////////////////////////
// Final Color Composition
//////////////////////////////////////////////////////////////////////////
void main(void)
{
	vec2 st = gl_TexCoord[0].st;

	float objectID = texture2D(idLayer, st).x;
	
	vec4 sceneColor = texture2D(sceneLayer, st);
	vec4 voronoiInfo = texture2D(voronoiLayer, st);

    vec3 normal = texture2D(normalDepthLayer, st).xyz;
    float depth = texture2D(normalDepthLayer, st).w;
    
    // insert your post processing here

    //gl_FragColor = sceneColor;

    // funny effect rings effect

   gl_FragColor = voronoiInfo; // uncomment to show only distance field

}