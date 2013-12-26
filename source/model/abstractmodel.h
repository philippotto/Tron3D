#pragma once
// STD
#include <vector>
#include <deque>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../forwarddeclarations.h"

namespace troen
{
	typedef enum enum_COLLISIONTYPE
	{
		ABSTRACTTYPE,
		BIKETYPE,
		LEVELTYPE,
		LEVELWALLTYPE,
		LEVELGROUNDTYPE,
		FENCETYPE
	} COLLISIONTYPE;

	// max 15 collision groups (enum -> short int)
	enum CollisionGroups {
		COLGROUP_NONE = 0,
		COLGROUP_BIKE = 1 << 0,
		COLGROUP_FENCE = 1 << 1,
		COLGROUP_LEVEL = 1 << 2
	};

	// specifies with which other groups an object can collide with
	// assuming its group is set in the add
	enum CollisionMasks {
		COLMASK_NONE = 0,
		COLMASK_BIKE = COLGROUP_BIKE | COLGROUP_FENCE | COLGROUP_LEVEL,
		COLMASK_FENCE = COLGROUP_BIKE,
		COLMASK_LEVEL = COLGROUP_BIKE,
	};

	class AbstractModel
	{
		public:
			AbstractModel();
			
			virtual std::vector<std::shared_ptr<btRigidBody>> getRigidBodies();

		protected:
			std::vector<std::shared_ptr<btRigidBody>> m_rigidBodies;
			std::vector<std::shared_ptr<btMotionState>> m_motionStates;
			std::vector<std::shared_ptr<btCollisionShape>> m_collisionShapes;
	};
}