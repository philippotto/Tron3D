/* BlurFragmentShader.glsl */

#version 130
// precision mediump float;

uniform float time;
uniform sampler2D colorTex;
uniform sampler2D inputLayer;
uniform sampler2D idLayer;


in vec2 v_texCoord;
in vec2 v_blurTexCoords[14];

in float doHorizontalBlur;
 
float sigma = 4 + 4 * (sin(time*1.5) + 1);     // The sigma value for the gaussian function: higher value means more blur
                         // A good value for 9x9 is around 3 to 5
                         // A good value for 7x7 is around 2.5 to 4
                         // A good value for 5x5 is around 2 to 3.5
                         // ... play around with this based on what you need :)


const float pi = 3.14159265f;


void main() {
    float blurSize;
    float numBlurPixelsPerSide;
    vec2  blurMultiplyVec;

    float objectID = texture2D(idLayer, v_texCoord).x;
    float glowIntensity = texture2D(idLayer, v_texCoord).y;

    if (doHorizontalBlur < 0.1f) {
        numBlurPixelsPerSide = 8.0f;
        blurMultiplyVec      = vec2(1.0f, 0.0f);

        blurSize = 1.f / 700.f * glowIntensity;  // This should usually be equal to
                         // 1.0f / texture_pixel_width for a horizontal blur, and
                         // 1.0f / texture_pixel_height for a vertical blur.
            
    }else {
        // return;
        numBlurPixelsPerSide = 8.0f;
        blurMultiplyVec      = vec2(0.0f, 1.0f);
        blurSize = 1.f / 700.f * glowIntensity;
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
  for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { 
    avgValue += texture2D(inputLayer, v_texCoord.xy - i * blurSize * 
                          blurMultiplyVec) * incrementalGaussian.x;         
    avgValue += texture2D(inputLayer, v_texCoord.xy + i * blurSize * 
                          blurMultiplyVec) * incrementalGaussian.x;         
    coefficientSum += 2 * incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;
  }

  gl_FragColor = avgValue / coefficientSum;
  // gl_FragColor = v_texCoord.xy;
}