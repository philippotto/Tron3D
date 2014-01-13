#pragma once
#ifndef _CONSTANTS_H
#define _CONSTANTS_H 
// OSG
#include <osg/Array>
#include "forwarddeclarations.h"

// http://stackoverflow.com/questions/3681140/how-do-i-avoid-both-global-variables-and-magic-numbers

namespace troen
{
	// GENERAL

	// GAME
	extern const int DEFAULT_WINDOW_WIDTH;
	extern const int DEFAULT_WINDOW_HEIGHT;
	extern const float DEFAULT_SOUND_VOLUME;

	extern const int NUM_MULTISAMPLES;

	extern const float FOVY_INITIAL;
	extern const float FOVY_ADDITION_MAX;
	extern const float FOVY_DELTA_MAX;

	//PHYSIS
	extern const btVector3 DEFAULT_GRAVITY;

	// BIKE
	extern const btVector3 BIKE_DIMENSIONS;
	extern const osg::Vec3f BIKE_VIEW_SCALE_FACTORS;
	extern const osg::Vec3f BIKE_VIEW_TRANSLATE_VALUES;

	extern const float BIKE_MASS;
	extern const int BIKE_VELOCITY_MAX;
	extern const int BIKE_VELOCITY_MIN;
	extern const float BIKE_VELOCITY_DAMPENING_TERM;
	extern const float BIKE_ACCELERATION_FACTOR_MAX;
	extern const float BIKE_TURN_FACTOR_MAX;
	extern const float BIKE_ANGULAR_DAMPENING_TERM;


	extern const float BIKE_TILT_DAMPENING;
	extern const float BIKE_TILT_MAX;

	//INPUT
	extern const float BIKE_MOVE_VALUE;
	extern const float BIKE_ROTATION_VALUE;
	extern const float BIKE_HANDBRAKE_FACTOR;
	extern const int POLLING_DELAY_MS;

	// FENCE
	// determines how accurate the fence will be
	extern const float FENCE_HEIGHT_MODEL;
	extern const float FENCE_HEIGHT_VIEW;
	extern const float FENCE_PART_LENGTH;
	extern const float FENCE_PART_WIDTH;
	extern const int DEFAULT_MAX_FENCE_PARTS;

	// CAMERA
	extern const osg::Vec3 CAMERA_POSITION_OFFSET;
	extern const float CAMERA_ROTATION_OFFSET;
	extern const osg::Vec3 CAMERA_EYE_POSITION;
	extern const float CAMERA_TILT_FACTOR;

	// PHYSICS
	extern const float BIKE_IMPACT_THRESHOLD_LOW;
	extern const float BIKE_IMPACT_THRESHOLD_HIGH;
}

#endif

// MACROS FROM CG

#include <cmath>

// NOTE: When using powers, try to use Horner's Method

#define PI 3.1415926535897932384626433832795L
#define PI2  (PI * 2.00L)
#define PI_2 (PI * 0.50L)
#define PI4  (PI * 4.00L)
#define PI_4 (PI * 0.25L)

#define round(v)\
	floor(v + 0.5)

#define abs(v) \
	(((v) < 0) ? -(v) : (v))

#define sign(v) \
	(((v) < 0) ? -1 : 1)

#define intcast(v) \
	(static_cast<int>(v))

#define shortcast(v) \
	(static_cast<short>(v))

#define mod(a, m) \
	((a)-(m)* (intcast((a) / (m)) - ((a) < 0 ? 1 : 0)))

#define frac(x) \
	((x)-intcast(x))

#define clamp(l, u, x) \
	((x) < (l) ? (l) : (x) > (u) ? (u) : (x))

#define deg(rad) \
	((rad)* 180.0L / PI)

#define rad(deg) \
	((deg)* PI / 180.0L)

#define mi(a, b) \
	((a) < (b) ? (a) : (b))

#define ma(a, b) \
	((a) < (b) ? (b) : (a))

// same as hour
// note: if d is negative use -decimal(d, m, s) instead of decimal(-d, m, s)
#define decimal(d, m, s) \
	((d)+((m)+(s) / 60.0L) / 60.0L)

#define sind(deg) \
	(sin(rad(deg)))

#define cosd(deg) \
	(cos(rad(deg)))

#define tand(deg) \
	(tan(rad(deg)))

#define asind(rad) \
	(deg(asin(rad)))

#define acosd(rad) \
	(deg(acos(rad)))

#define atand(rad) \
	(deg(atan(rad)))

#define atan2d(x, y) \
	(deg(atan2(x, y)))

#define adiameter(D, r) \
	(2 * atan(r / D))

// normalizes an angle to between 0 and 2PI radians
#define rev(rad) \
	((rad)-floor((rad) / PI2) * PI2)

// normalizes an angle to between 0 and 360 degrees
#define revd(deg) \
	((deg)-floor((deg) / 360.0L) * 360.0L)

// cube root (e.g. needed for parabolic orbits)
#define cbrt(x) \
	(((x) > 0.0) ? exp(log(x) / 3.0L) : (((x) < 0.0) ? -cbrt(-(x)) : 0.0))


#define __b02(x) (     (x) | (     (x) >>  1))
#define __b04(x) (__b02(x) | (__b02(x) >>  2))
#define __b08(x) (__b04(x) | (__b04(x) >>  4))
#define __b16(x) (__b08(x) | (__b08(x) >>  8))
#define __b32(x) (__b16(x) | (__b16(x) >> 16))

// Returns the next power of an integer.
#define nextPowerOf2(x) \
	(__b32((x)-1) + 1)

#define prevPowerOf2(x) \
	(nextPowerOf2(x) >> 1)

#define randf(min, max) \
	(static_cast<float>(rand()) / RAND_MAX * ((max)-(min)) + (min))

#define _rand(min, max) \
	(static_cast<int>(static_cast<float>(rand()) / RAND_MAX * ((max)-(min)) + (min)))


// Interpolate

#define linear(t)       (t)

#define smoothstep(t)   ((t) * (t) * (3 - 2 * (t)))
#define smoothstep2(t)  (smoothstep(smoothstep(t)))
#define smoothstep3(t)  (smoothstep(smoothstep2(t)))

#define smootherstep(t) ((t) * (t) * (t) * ((t) * (6 * (t) - 15) + 10))

#define squared(t)      ((t) * (t))
#define invsquared(t)   (1 - (1 - (t)) * (1 - (t)))

#define cubed(t)        ((t) * (t) * (t))
#define invcubed(t)     (1 - (1 - (t)) * (1 - (t)) * (1 - (t)))

#define sin(t)          (sin(t * 1.57079632679489661923))
#define invsin(t)       (1 - sin((1 - (t)) * 1.57079632679489661923))


#define smoothstep_ext(t, l, r) \
	((t) < (l) ? 0 : (r) < (t) ? 1 : smoothstep(((t)-(l)) / ((r)-(l))))

// Several interpolation methods in action: http://sol.gfxile.net/interpolation/

enum InterpolationMethod
{
	InterpolateLinear
	, InterpolateSmoothStep
	, InterpolateSmoothStep2
	, InterpolateSmoothStep3
	, InterpolateSmootherStep // Ken Perlin
	, InterpolateSquared
	, InterpolateInvSquared
	, InterpolateCubed
	, InterpolateInvCubed
	, InterpolateSin          // strong in, soft out
	, InterpolateInvSin       // soft in, strong out
};


template<typename T>
inline const T interpolate(
	const T t
	, const InterpolationMethod function = InterpolateLinear)
{
	switch (function)
	{
	case InterpolateSmoothStep:
		return smoothstep(t);
	case InterpolateSmoothStep2:
		return smoothstep2(t);
	case InterpolateSmoothStep3:
		return smoothstep3(t);
	case InterpolateSmootherStep:
		return smootherstep(t);
	case InterpolateSquared:
		return squared(t);
	case InterpolateInvSquared:
		return invsquared(t);
	case InterpolateCubed:
		return cubed(t);
	case InterpolateInvCubed:
		return invcubed(t);
	case InterpolateSin:
		return static_cast<T>(sin(t));
	case InterpolateInvSin:
		return static_cast<T>(invsin(t));
	default:
	case InterpolateLinear:
		return linear(t);
	}
}