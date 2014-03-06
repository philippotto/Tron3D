#include "constants.h"
// bullet
#include "btBulletDynamicsCommon.h"
#include <osg/Array>

// http://stackoverflow.com/questions/3681140/how-do-i-avoid-both-global-variables-and-magic-numbers

namespace troen
{
	// GENERAL

	// GAME
	const int DEFAULT_WINDOW_WIDTH(1024);
	const int DEFAULT_WINDOW_HEIGHT(768);
	const float DEFAULT_SOUND_VOLUME(1.f);

	const float FOVY_INITIAL(29.1484f);
	const float FOVY_ADDITION_MAX(30.f);
	const float FOVY_DELTA_MAX(.7f);

	const int NUM_MULTISAMPLES(8);

	// LOGIC
	const double RESPAWN_DURATION(3000);
	const double GAME_START_COUNTDOWN_DURATION(3000);

	//PHYSIS
	const btVector3 DEFAULT_GRAVITY(0,0,-98);

	// BIKE
	const btVector3 BIKE_DIMENSIONS(2, 4, 2);
	const float BIKE_VIEW_SCALE_FACTOR(1.f / 5.f * BIKE_DIMENSIONS.y());
	const osg::Vec3f BIKE_VIEW_SCALE_FACTORS(BIKE_VIEW_SCALE_FACTOR, BIKE_VIEW_SCALE_FACTOR, BIKE_VIEW_SCALE_FACTOR);
	const osg::Vec3f BIKE_VIEW_TRANSLATE_VALUES(0,0,-BIKE_DIMENSIONS.z() * 4.9f / 12.5);

	const float BIKE_MASS(300);
	const int BIKE_VELOCITY_MAX(BIKE_DIMENSIONS.y() * 90);
	const int BIKE_TURBO_VELOCITY_MAX(BIKE_VELOCITY_MAX * 1.7);
	const int BIKE_VELOCITY_MIN(BIKE_DIMENSIONS.y() * 30);
	const float BIKE_VELOCITY_DAMPENING_TERM(.3f);
	const float BIKE_ACCELERATION_FACTOR_MAX(1.5f);
	const float BIKE_DECELERATION_FACTOR(4.f);
	const float BIKE_TURN_FACTOR_MAX(15);
	const float BIKE_ANGULAR_DAMPENING_TERM(0.001f);
	const float BIKE_VIEWING_ANGLE_DAMPENING_TERM(0.1f);

	const float TURBO_PHASE_LENGTH(2000);

	// BIKE_TILT_DAMPENING = 1 would lead to immediate/unsmooth tilt
	// 1 / BIKE_TILT_MAX specifies angle in radiant
	const float BIKE_TILT_DAMPENING(20.f);
	const float BIKE_TILT_MAX(BIKE_ROTATION_VALUE + BIKE_ROTATION_VALUE * BIKE_HANDBRAKE_FACTOR);
	const float BIKE_WHEELY_TILT_MAX(2.f);
	const float THRESHOLD_FOR_ABRUPT_VELOCITY_CHANGE(20.f);

	//INPUT
	const float BIKE_ROTATION_VALUE(12.0f);
	const float BIKE_HANDBRAKE_FACTOR(1.4f);
	// this should always be less than 1000/60FPS - the smaller, the more responsive is the input
	const int POLLING_DELAY_MS(8);
	const int VIBRATION_TIME_MS(500);

	// LEVEL
	const int LEVEL_SIZE(6000);

	// FENCE
	// determines how accurate the fence will be
	const float FENCE_HEIGHT_MODEL(BIKE_DIMENSIONS.x() * 1.5f);
	const float FENCE_HEIGHT_VIEW(FENCE_HEIGHT_MODEL * .7f);
	const float FENCE_PART_LENGTH(BIKE_DIMENSIONS.y() / 2);
	const float FENCE_PART_WIDTH(BIKE_DIMENSIONS.x() * .3f);
	const int DEFAULT_MAX_FENCE_PARTS(400);

	// CAMERA
	const osg::Vec3 CAMERA_POSITION_OFFSET(0, 0, BIKE_DIMENSIONS.y());
	const float CAMERA_ROTATION_OFFSET(.05f);
	const osg::Vec3 CAMERA_EYE_POSITION(0.f,-BIKE_DIMENSIONS.y()*5.5f,BIKE_DIMENSIONS.z()*.8f);
	const float CAMERA_TILT_FACTOR(4.f);
	const int HUD_PROJECTION_SIZE(1000);


	const unsigned int CAMERA_MASK_MAIN(1 << 0);
	const unsigned int CAMERA_MASK_RADAR(1 << 1);
	const unsigned int CAMERA_MASK_PLAYER[6] {1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7};
	const unsigned int CAMERA_MASK_NONE(0);



	// PHYSICS
	const float BIKE_FENCE_IMPACT_THRESHOLD_LOW(BIKE_MASS*BIKE_VELOCITY_MIN);
	const float BIKE_FENCE_IMPACT_THRESHOLD_HIGH(BIKE_MASS*BIKE_VELOCITY_MAX / 3);


	const float BIKE_DEFAULT_HEALTH(3 * BIKE_FENCE_IMPACT_THRESHOLD_HIGH);

	// AUDIO
	const int ENGINE_FREQUENCY_LOW(50000);
	const int ENGINE_FREQUENCY_HIGH(120000);

}