#version 130

uniform float time;
uniform float timeSinceLastBeat;
uniform sampler2D inputLayer;
uniform sampler2D idLayer;

in vec2 v_texCoord;

in float doHorizontalBlur;

// The sigma value for the gaussian function: higher value means more blur
// good values are:
// 9x9: 3 to 5; 7x7: 2.5 to 4, 5x5: 2 to 3.5
// should be between 4 and 12
float sigma =
		+ 4
		+ 3 * (sin(time*2.f) + 1.f)/2.f
		+ 4 * max(10, 1000 - timeSinceLastBeat) / 1000;

const float pi = 3.14159265f;

void main() {

	float blurSize;
	float numBlurPixelsPerSide;
	float glowIntensity = texture2D(idLayer, v_texCoord).y;

	if (glowIntensity == 0.f)
		glowIntensity = 1.f;

	vec2 textureSize = textureSize(inputLayer, 0);

	vec2 blurMultiplyVec;
	if (doHorizontalBlur == 0.0f)
	{
		numBlurPixelsPerSide = 16.0f;
		blurMultiplyVec      = vec2(1.0f, 0.0f);

		// This should usually be equal to
		// 1.0f / texture_pixel_width (or _height) for a horizontal (vertical) blur
		blurSize = 1.f / textureSize.y * glowIntensity;
	} else {
		numBlurPixelsPerSide = 16.0f;
		blurMultiplyVec      = vec2(0.0f, 1.0f);
		blurSize = 1.f / textureSize.x * glowIntensity;
	}

	// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
	vec3 incrementalGaussian;
	incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);
	incrementalGaussian.y = exp(-0.5f / (sigma * sigma));
	incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

	vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	float coefficientSum = 0.0f;

	// Take the central sample first...
	avgValue += texture2D(inputLayer, v_texCoord.xy) * incrementalGaussian.x;
	coefficientSum += incrementalGaussian.x;
	incrementalGaussian.xy *= incrementalGaussian.yz;

	// Go through the remaining 8 vertical samples (4 on each side of the center)
	for (float i = 1.0f; i <= numBlurPixelsPerSide; i++)
	{
		avgValue += texture2D(inputLayer, v_texCoord.xy - i * blurSize *
						  blurMultiplyVec) * incrementalGaussian.x;
		avgValue += texture2D(inputLayer, v_texCoord.xy + i * blurSize *
						  blurMultiplyVec) * incrementalGaussian.x;
		coefficientSum += 2 * incrementalGaussian.x;
		incrementalGaussian.xy *= incrementalGaussian.yz;
	}

	gl_FragColor = avgValue / coefficientSum;
	return;
}