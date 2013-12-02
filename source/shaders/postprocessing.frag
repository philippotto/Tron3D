#version 120

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
    const int rings = 20;
    int test = int(voronoiInfo.w*rings);

    float value = voronoiInfo.w*rings - test;
    vec4 ringColor = vec4(1,voronoiInfo.x, value,1);

    if (voronoiInfo.w > 0.01)
    {   
        gl_FragColor = ringColor;
    }else
    {
        gl_FragColor = mix(sceneColor, ringColor, voronoiInfo.w / 0.01);
    }

    //gl_FragColor = vec4(vec3(voronoiInfo.w), 1.0); // uncomment to show only distance field
    
}