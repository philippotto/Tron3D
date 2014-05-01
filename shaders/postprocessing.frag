#version 130

uniform sampler2D sceneLayer;
uniform sampler2D idLayer;
uniform sampler2D pongLayer;
uniform sampler2D oldLayer;
uniform float healthNormalized;
uniform float time;
uniform float velocity;
uniform float timeFactor;
uniform float timeSinceLastHit;

//////////////////////////////////////////////////////////////////////////
// Final Color Composition
//////////////////////////////////////////////////////////////////////////
void main(void)
{
	vec2 st = gl_TexCoord[0].st;

	vec4 sceneColor = texture2D(sceneLayer, st);


	// blur again to eliminate pixelation which is caused by implicit upscaling
	const float diff = 0.001;
	vec4 pongColor =  (4 * texture2D(pongLayer, st)
		+ texture2D(pongLayer, st + vec2(diff))
		+ texture2D(pongLayer, st + vec2(-diff))
		+ texture2D(pongLayer, st + vec2(diff, -diff))
		+ texture2D(pongLayer, st + vec2(-diff, diff))
	) / 8;

	vec4 oldColor = texture2D(oldLayer, st);

	// motion blur is framerate independent
	float motionBlurFactor = smoothstep(300.0, 450.0, velocity);
	float oldFrameWeight = max(0.1, pow(motionBlurFactor * 0.8, timeFactor));
	float newFrameWeight = 1 - oldFrameWeight;

	st = 1 * (st - vec2(0.5));

	float enableDamageHUD = healthNormalized;
	float circleTransparency = st.x * st.x + st.y * st.y;
	float pulsating = (sin(time*2.f) + 1.f)/3.f + 0.4 ;
	circleTransparency = (1-enableDamageHUD)*pulsating *  min(1, 2 * circleTransparency);
	float hitPulse = 20.0 / max(timeSinceLastHit, 50);
	circleTransparency += hitPulse;

	vec3 reddedColor = vec3(1.0,0.0,0.0);


	vec4 newColor = sceneColor + pongColor;
	newColor = mix(newColor, vec4(reddedColor, 1.f), circleTransparency);

	gl_FragColor =newFrameWeight * newColor + oldFrameWeight * oldColor;//texture2D(idLayer, st);//

	return;
}