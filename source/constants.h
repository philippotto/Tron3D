#pragma once
#ifndef _CONSTANTS_H
#define _CONSTANTS_H 
// OSG
#include <osg/Array>
#include "forwarddeclarations.h"

// http://stackoverflow.com/questions/3681140/how-do-i-avoid-both-global-variables-and-magic-numbers

// comment out to disable debug mode
//#define DEBUG_DRAW

namespace troen
{
	// GENERAL
	extern const double PI;

	// GAME
	extern const int DEFAULT_WINDOW_WIDTH;
	extern const int DEFAULT_WINDOW_HEIGHT;
	extern const float DEFAULT_SOUND_VOLUME;

	//PHYSIS
	extern const btVector3 DEFAULT_GRAVITY;

	// BIKE
	extern const btVector3 BIKE_DIMENSIONS;
	extern const float BIKE_VIEW_SCALE_FACTOR;
	extern const osg::Vec3f BIKE_VIEW_SCALE_FACTORS;
	extern const osg::Vec3f BIKE_VIEW_TRANSLATE_VALUES;

	extern const float BIKE_MASS;
	extern const int BIKE_MAX_VELOCITY;
	//INPUT
	extern const float BIKE_MOVE_VALUE;
	extern const float BIKE_ROTATION_VALUE;

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
}

#endif