#pragma once

#include "btBulletDynamicsCommon.h"

// 
// http://stackoverflow.com/questions/3681140/how-do-i-avoid-both-global-variables-and-magic-numbers
//

// comment out to disable debug mode
//#define DEBUG_DRAW

namespace troen
{
	const double PI(3.14159265359);

	const int DEFAULT_WINDOW_WIDTH(1280);
	const int DEFAULT_WINDOW_HEIGHT(720);
	const float DEFAULT_SOUND_VOLUME(1.f);

	const int DEFAULT_MAX_FENCE_PARTS(150);

	// determines how accurate the fence will be
	const float DEFAULT_FENCE_LENGTH(15);

	const btVector3 DEFAULT_BIKE_DIMENSIONS(12.5, 25, 12.5);

}