#include "constants.h"
// bullet
#include "btBulletDynamicsCommon.h"
#include <osg/Array>

// http://stackoverflow.com/questions/3681140/how-do-i-avoid-both-global-variables-and-magic-numbers

namespace troen
{
	// GENERAL
	const double PI(3.14159265359);

	// GAME
	const int DEFAULT_WINDOW_WIDTH(1280);
	const int DEFAULT_WINDOW_HEIGHT(720);
	const float DEFAULT_SOUND_VOLUME(1.f);

	//PHYSIS
	const btVector3 DEFAULT_GRAVITY(0,0,-10);

	// BIKE
	const btVector3 BIKE_DIMENSIONS(2, 4, 2);
	const float BIKE_VIEW_SCALE_FACTOR(1.f / 5.f * BIKE_DIMENSIONS.y());
	const osg::Vec3f BIKE_VIEW_SCALE_FACTORS(BIKE_VIEW_SCALE_FACTOR, BIKE_VIEW_SCALE_FACTOR, BIKE_VIEW_SCALE_FACTOR);
	const osg::Vec3f BIKE_VIEW_TRANSLATE_VALUES(0,0,-BIKE_DIMENSIONS.z() * 4.9f / 12.5);

	const float BIKE_MASS(300);
	const int BIKE_MAX_VELOCITY(BIKE_DIMENSIONS.y() * 40);
	const int BIKE_MIN_VELOCITY(BIKE_DIMENSIONS.y() * 10);

	// BIKE_TILT_DAMPENING = 1 would lead to immediate/unsmooth tilt
	// 1 / BIKE_TILT_MAX specifies angle in radiant
	const float BIKE_TILT_DAMPENING(20.f);
	const float BIKE_TILT_MAX(16.f);

	//INPUT
	const float BIKE_MOVE_VALUE(2.0);
	const float BIKE_ROTATION_VALUE(10.0);

	// FENCE
	// determines how accurate the fence will be
	const float FENCE_HEIGHT_MODEL(BIKE_DIMENSIONS.x() * 1.5f);
	const float FENCE_HEIGHT_VIEW(FENCE_HEIGHT_MODEL * .7f);
	const float FENCE_PART_LENGTH(BIKE_DIMENSIONS.y() / 2);
	const float FENCE_PART_WIDTH(BIKE_DIMENSIONS.x() * .3f);
	const int DEFAULT_MAX_FENCE_PARTS(150);

	// CAMERA
	const osg::Vec3 CAMERA_POSITION_OFFSET(0, 0, BIKE_DIMENSIONS.y());
	const float CAMERA_ROTATION_OFFSET(.05f);
	const int debugNormalizer = 1;
#ifdef _DEBUG
	debugNormalizer = -1;
#endif
	const osg::Vec3 CAMERA_EYE_POSITION(0.f,debugNormalizer * -BIKE_DIMENSIONS.y()*5.5f,BIKE_DIMENSIONS.z()*.8f);
	const float CAMERA_TILT_FACTOR(16.f);

	// PHYSICS
	const float BIKE_IMPACT_THRESHOLD_LOW(BIKE_MASS*BIKE_MIN_VELOCITY);
	const float BIKE_IMPACT_THRESHOLD_HIGH(BIKE_MASS*BIKE_MAX_VELOCITY);
}