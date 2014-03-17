#include "levelmodel.h"
//bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"
//troen
#include "../controller/itemcontroller.h"
#include "objectinfo.h"
#include "../constants.h"
#include "../controller/abstractcontroller.h"
#include "../controller/levelcontroller.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace troen;

LevelModel::LevelModel(const LevelController* levelController, std::string levelName)
{
	AbstractModel();
	m_levelController = levelController;
	m_rigidBodies = std::vector<std::shared_ptr<btRigidBody>>();

	addFloor(-10);
	addObstaclesFromFile(levelName);
}

void LevelModel::reload(std::string levelName)
{
	m_rigidBodies.clear();
	m_motionStates.clear();
	m_collisionShapes.clear();
	m_obstacles.clear();
	m_floors.clear();

	addFloor(-10);
	addObstaclesFromFile(levelName);
}

void LevelModel::addObstaclesFromFile(std::string levelName)
{
	std::ifstream input("data/levels/" + levelName + ".level");
	std::string line;

	btVector3 center, dimensions;
	btQuaternion rotation;
	std::string name, collisionTypeString;

	std::vector<BoxModel> newObstacles;

	while (std::getline(input, line)) {

		QString qLine;

		double x, y, z, w;

		// center

		qLine = QString::fromStdString(line);
		x = qLine.toDouble();

		std::getline(input, line);
		qLine = QString::fromStdString(line);
		y = qLine.toDouble();

		std::getline(input, line);
		qLine = QString::fromStdString(line);
		z = qLine.toDouble();

		center = btVector3(x, y, z);


		// dimensions

		std::getline(input, line);
		qLine = QString::fromStdString(line);
		x = qLine.toDouble();

		std::getline(input, line);
		qLine = QString::fromStdString(line);
		y = qLine.toDouble();

		std::getline(input, line);
		qLine = QString::fromStdString(line);
		z = qLine.toDouble();

		dimensions = btVector3(x, y, z);


		// rotation

		std::getline(input, line);
		qLine = QString::fromStdString(line);
		x = qLine.toDouble();

		std::getline(input, line);
		qLine = QString::fromStdString(line);
		y = qLine.toDouble();

		std::getline(input, line);
		qLine = QString::fromStdString(line);
		z = qLine.toDouble();

		std::getline(input, line);
		qLine = QString::fromStdString(line);
		w = qLine.toDouble();

		rotation = btQuaternion(x, y, z, w);


		// name, collisionType

		std::getline(input, line);
		name = line;

		std::getline(input, line);
		collisionTypeString = line;

		std::string collisionTypes[8] = { "ABSTRACTTYPE", "BIKETYPE", "LEVELTYPE", "LEVELWALLTYPE", "LEVELGROUNDTYPE", "LEVELOBSTACLETYPE", "FENCETYPE", "ITEMTYPE" };
		int index = 0;
		for (auto type : collisionTypes) {
			if (type == collisionTypeString)
				break;
			index++;
		}

		troen::COLLISIONTYPE collisionType = static_cast<troen::COLLISIONTYPE>(index);

		BoxModel newBox(center, dimensions, rotation, name, collisionType);
		newObstacles.push_back(newBox);
	}

	m_obstacles.insert(m_obstacles.end(), newObstacles.begin(), newObstacles.end());
	addBoxes(m_obstacles);
}


void LevelModel::addFloor(float yPosition)
{
	btScalar size = getLevelSize();
	m_floors.push_back({
		btVector3(0, 0, yPosition),
		btVector3(size, size, 20)
	});

	addBoxes(m_floors, LEVELGROUNDTYPE);
}

void LevelModel::addBoxes(std::vector<BoxModel> &boxes, COLLISIONTYPE type)
{
	for (int i = 0; i < boxes.size(); ++i)
	{
		std::shared_ptr<btBoxShape> wallShape = std::make_shared<btBoxShape>(boxes[i].dimensions / 2);
		std::shared_ptr<btDefaultMotionState> wallMotionState
			= std::make_shared<btDefaultMotionState>(btTransform(boxes[i].rotation, boxes[i].center));

		btRigidBody::btRigidBodyConstructionInfo
			wallRigidBodyCI(btScalar(0), wallMotionState.get(), wallShape.get(), btVector3(0, 0, 0));

		std::shared_ptr<btRigidBody> wallRigidBody = std::make_shared<btRigidBody>(wallRigidBodyCI);

		ObjectInfo* info;
		if (type == ABSTRACTTYPE)
			info = new ObjectInfo(const_cast<LevelController*>(m_levelController), boxes[i].collisionType);
		else
			info = new ObjectInfo(const_cast<LevelController*>(m_levelController), type);

		wallRigidBody->setUserPointer(info);

		m_collisionShapes.push_back(wallShape);
		m_motionStates.push_back(wallMotionState);
		m_rigidBodies.push_back(wallRigidBody);
	}
}

btScalar LevelModel::getLevelSize()
{
	return btScalar(LEVEL_SIZE);
}
