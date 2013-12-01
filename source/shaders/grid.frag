#version 130

in vec2 uv;

void main()
{
	// Apply a procedural texture here.

	//float i = ... ; 
	int inverseFrequency = 100;
	
	// float xBlack = abs(sign(mod(floor(5000 * uv.x), inverseFrequency)));
	// float yBlack = abs(sign(mod(floor(5000 * uv.y), inverseFrequency)));

	float modifier = 2;
	float smoothingFactorX = abs(mod((5000 / modifier * uv.x), inverseFrequency / modifier) - 5);
	float smoothingFactorY = abs(mod((5000 / modifier * uv.y), inverseFrequency / modifier) - 5);
	
	gl_FragColor = mix(
		vec4(22, 115, 131, 1) / 255.f,
		vec4(0, 0, 0, 1) / 255.f,
		pow(min(smoothingFactorX, smoothingFactorY), 0.5)
	);


}
