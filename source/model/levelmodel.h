#pragma once
//troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"
#include <btBulletDynamicsCommon.h>

namespace troen
{
	// helper class to abstract boxes from bullet and osg

	class BoxModel {
	public:
		BoxModel(btVector3 centerVec, btVector3 dimensionsVec) {
			center = centerVec;
			dimensions = dimensionsVec;
			rotation = btQuaternion(0, 0, 0, 1);
			name = "default";
			collisionType = 1;

		};

		BoxModel(btVector3 centerVec, btVector3 dimensionsVec, btQuaternion rotationQuat) {
			center = centerVec;
			dimensions = dimensionsVec;
			rotation = rotationQuat;
			name = "default";
			collisionType = 1;
		}

		BoxModel(btVector3 centerVec, btVector3 dimensionsVec, btQuaternion rotationQuat, std::string modelName, int collision_type) {
			center = centerVec;
			dimensions = dimensionsVec;
			rotation = rotationQuat;
			name = modelName;
			collisionType = collision_type;
		}

		btVector3 center;
		btVector3 dimensions;
		btQuaternion rotation;
		std::string name;
		int collisionType;
	};

	
	class LevelModel : public AbstractModel
	{
	public:
		LevelModel(const LevelController* levelController);
		int getLevelSize();

		std::vector<BoxModel>& getWalls() { return m_walls; };
		std::vector<BoxModel>& getFloors() { return m_floors; };
		std::vector<BoxModel>& getObstacles() { return m_obstacles; };

		btCollisionObject* createItemBox(btVector3 &position);

	private:
		void addWalls(float levelSize, float yPosition);
		void addFloor(float size, float yPosition);
		void addBoxes(std::vector<BoxModel> &boxes, COLLISIONTYPE type);

		void auto_addObstacles();

		const LevelController* m_levelController;

		std::vector<BoxModel> m_walls;
		std::vector<BoxModel> m_floors;
		std::vector<BoxModel> m_obstacles;
	};

}