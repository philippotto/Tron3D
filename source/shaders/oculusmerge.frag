#version 130

uniform sampler2D left;
uniform sampler2D right;

//////////////////////////////////////////////////////////////////////////
// Final Color Composition
// this shader will be called for each eye
//////////////////////////////////////////////////////////////////////////

uniform vec2 LensCenter;
uniform vec2 ScreenCenter;
uniform vec2 Scale;
uniform vec2 ScaleIn;
uniform vec4 HmdWarpParam;




// varying vec2 Texcoord;






void main(void)
{
vec4 warpParameters = vec4(1.0f, 0.22f, 0.24f, 0.0f);
	vec2 Texcoord = gl_TexCoord[0].st;

	vec2 LensCenter = vec2(0.5f, 0.5f);
	vec2 ScreenCenter = vec2(0.5f, 0.5f);

	float lensSeparationDistance = 0.0635f;
	float vScreenResolution = 800;
	float hScreenResolution = 1280;
	float hScreenSize = 0.14976f;

	float aspectRatio = (hScreenResolution / 2 ) / vScreenResolution;

	float lensShift = hScreenSize * 0.25f - lensSeparationDistance * 0.5f;
	float lensViewportShift = 4.0f * lensShift / hScreenSize;
	float fitRadius = abs(-1 - lensViewportShift);
	float rsq = fitRadius*fitRadius;

	vec4 k = warpParameters;
	float scale = (k.x + k.y * rsq + k.z * rsq * rsq + k.w * rsq * rsq * rsq);


	float distortionScale = scale;
	float scaleFactor = 1.0f / distortionScale;
	vec2 Scale = vec2(0.5 * scaleFactor, 0.5f * scaleFactor * aspectRatio);

	vec2 ScaleIn = vec2(2.0f, 2.0f / aspectRatio);
	vec4 HmdWarpParam = warpParameters;



	// Scales input texture coordinates for distortion.
	vec2 theta = (Texcoord - LensCenter) * ScaleIn; // Scales texture coordinates to [-1, 1]
	float rSq = theta.x * theta.x + theta.y * theta.y;
	vec2 rvector= theta * (	HmdWarpParam.x +
							HmdWarpParam.y * rSq +
							HmdWarpParam.z * rSq * rSq +
							HmdWarpParam.w * rSq * rSq * rSq);


	vec2 tc = LensCenter + Scale * rvector;

	if (any(bvec2(clamp(tc, ScreenCenter - vec2(0.5, 0.5), ScreenCenter + vec2(0.5, 0.5)) - tc))) {
		gl_FragColor = vec4(0, 0, 0, 0);
	} else {
		// side = warpTexture

		if (tc.x == 0.0)
			gl_FragColor = vec4(1.f);
		else
			gl_FragColor = vec4(0.5);
		gl_FragColor = texture2D(left, tc) + texture2D(right, tc);
	}

	// gl_FragColor = vec4(1.f);
	return;
}



void mainOld(void)
{
	vec2 st = gl_TexCoord[0].st;

	vec4 color = texture2D(left, vec2(st.x, st.y));

	gl_FragColor = color;
}