
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
				btVector3(-10.268763303756714, -301.5158462524414,72.03860759735107),
				btVector3(529.6404647827148, 461.7601776123047, 12.254399061203003),
				btQuaternion(-4.367650063841211e-08,-0.9845066666603088,0.17534898221492767,-1.7463129831085666e-09),
                std::string("Cube.1387"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(-10.782455205917358, -778.228759765625,151.9006633758545),
				btVector3(531.1703491210938, 531.1703491210938, 8.43999981880188),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1386"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(988.9772033691406, 162.55023956298828,150.13311386108398),
				btVector3(288.5529136657715, 2405.4783630371094, 11.89049243927002),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1385"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(568.7586212158203, -879.2779541015625,146.33319854736328),
				btVector3(683.0246734619141, 321.9303894042969, 19.53296184539795),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1384"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(342.8825378417969, 918.4856414794922,153.36544036865234),
				btVector3(1020.994873046875, 898.2110595703125, 6.920419335365295),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1383"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(-115.997314453125, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1340"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-115.997314453125, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1339"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-115.997314453125, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1338"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-115.997314453125, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1337"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-115.997314453125, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1336"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-115.997314453125, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1335"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(209.60830688476562, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1330"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(209.60830688476562, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1329"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(94.06496047973633, -8.4912109375,10.0),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1328"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(209.60830688476562, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1327"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(209.60830688476562, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1326"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(209.60830688476562, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1325"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(535.2140808105469, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1320"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(535.2140808105469, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1319"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(752.2522735595703, 374.6894073486328,294.09372329711914),
				btVector3(184.14979934692383, 184.14979934692383, 600.5661010742188),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1318"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(535.2140808105469, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1317"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(535.2140808105469, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1316"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(535.2140808105469, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1315"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(860.8195495605469, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1310"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(860.8195495605469, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1309"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(860.8195495605469, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1308"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(860.8195495605469, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1307"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(860.8195495605469, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1306"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(860.8195495605469, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1305"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1186.4251708984375, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1300"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1186.4251708984375, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1299"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1186.4251708984375, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1298"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1186.4251708984375, 427.5050354003906,7.228968143463135),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1297"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1186.4251708984375, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1296"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1186.4251708984375, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1295"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(746.1251831054688, -637.6168060302734,292.9811477661133),
				btVector3(184.14979934692383, 184.14979934692383, 600.5661010742188),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1283"),
                LEVELOBSTACLETYPE
			} 

			};
			m_obstacles.insert(m_obstacles.end(), newObstacles.begin(), newObstacles.end());
		
			addBoxes(m_obstacles);
		}
	