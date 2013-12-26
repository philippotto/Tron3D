
		#include "levelmodel.h"
		//bullet
		#include <btBulletDynamicsCommon.h>
		#include "LinearMath/btHashMap.h"

		using namespace troen;

		//!!!!!!!!!!!!! WARNING: AUTO_GENERATED !!!!!!!!!!!!!!!!!!!!!!
		// If you want to change something generally, please edit obstacle_export.py, otherwise be sure to mark changes to this code otherwise it might be overwritten


		void LevelModel::auto_addObstacles()
		{
			// obstacles
			// TODO grab the value from origin
			std::vector<BoxModel> newObstacles = {

			
			{
				btVector3(342.88257598876953, 918.4856414794922,71.01015567779541),
				btVector3(1020.994873046875, 898.2110595703125, 6.920419335365295),
				btQuaternion(0.0,0.0,0.0,1.0)
			},

			{
				btVector3(70.70180892944336, -5.369071960449219,-5.070757865905762),
				btVector3(20.000009536743164, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0)
			},

			{
				btVector3(568.7586212158203, -879.2779541015625,66.1803674697876),
				btVector3(683.0246734619141, 321.9303894042969, 19.53296184539795),
				btQuaternion(0.0,0.0,0.0,1.0)
			},

			{
				btVector3(988.9772033691406, 162.55027770996094,69.9608039855957),
				btVector3(288.5529136657715, 2405.4783630371094, 11.89049243927002),
				btQuaternion(0.0,0.0,0.0,1.0)
			},

			{
				btVector3(-10.782440900802612, -778.228759765625,72.63442039489746),
				btVector3(531.1703491210938, 531.1703491210938, 8.442516922950745),
				btQuaternion(0.0,0.0,0.0,1.0)
			},

			{
				btVector3(-10.782440900802612, -264.0263366699219,30.399227142333984),
				btVector3(278.0601501464844, 516.7617797851562, 9.876872301101685),
				btQuaternion(-2.33497376989078e-09,0.08276040852069855,0.9965696334838867,-4.364898131825612e-08)
			},

			{
				btVector3(-80.16692161560059, -5.369071960449219,-5.070757865905762),
				btVector3(20.000009536743164, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0)
			} 

			};
			m_obstacles.insert(m_obstacles.end(), newObstacles.begin(), newObstacles.end());
		
			addBoxes(m_obstacles);
		}
	