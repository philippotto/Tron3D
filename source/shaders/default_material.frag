#version 130

uniform int modelID;
uniform float glowIntensity;
uniform vec4 materialColor;
uniform vec3 ambientColor;

in vec2 uv;

void main() {
	
	vec4 ambient_color = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;
	
	gl_FragData[0] = vec4(1.0,0.5,0.5,0.5);// gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	//2 float channels: select_group, attribute (f.e glowintensity for glow group)
	gl_FragData[1] = vec4(modelID,1.0,0,0);
}