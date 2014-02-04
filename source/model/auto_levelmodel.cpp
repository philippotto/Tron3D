
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
				btVector3(1512.0306396484375, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1388"),
                int(1)
			},

			{
				btVector3(-10.268763303756714, -301.5158462524414,72.03860759735107),
				btVector3(529.6404647827148, 461.7601776123047, 12.254399061203003),
				btQuaternion(-4.367650063841211e-08,-0.9845066666603088,0.17534898221492767,-1.7463129831085666e-09),
                std::string("Cube.1387"),
                int(0)
			},

			{
				btVector3(-10.782455205917358, -778.228759765625,151.9006633758545),
				btVector3(531.1703491210938, 531.1703491210938, 8.43999981880188),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1386"),
                int(0)
			},

			{
				btVector3(988.9772033691406, 162.55023956298828,150.13311386108398),
				btVector3(288.5529136657715, 2405.4783630371094, 11.89049243927002),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1385"),
                int(0)
			},

			{
				btVector3(568.7586212158203, -879.2779541015625,146.33319854736328),
				btVector3(683.0246734619141, 321.9303894042969, 19.53296184539795),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1384"),
                int(0)
			},

			{
				btVector3(342.8825378417969, 918.4856414794922,152.43303298950195),
				btVector3(1020.994873046875, 898.2110595703125, 6.920419335365295),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1383"),
                int(0)
			},

			{
				btVector3(-1418.4197998046875, -1752.4765014648438,9.99596655368805),
				btVector3(20.000009536743164, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1382"),
                int(1)
			},

			{
				btVector3(-1418.4197998046875, -1316.4801025390625,9.99596655368805),
				btVector3(20.000009536743164, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1381"),
                int(1)
			},

			{
				btVector3(-1418.4197998046875, -880.4838562011719,9.99596655368805),
				btVector3(20.000009536743164, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1380"),
                int(1)
			},

			{
				btVector3(-1418.4197998046875, -444.48760986328125,9.99596655368805),
				btVector3(20.000009536743164, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1379"),
                int(1)
			},

			{
				btVector3(-1418.4197998046875, -8.4912109375,9.99596655368805),
				btVector3(20.000009536743164, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1378"),
                int(1)
			},

			{
				btVector3(-1418.4197998046875, 427.5050354003906,9.99596655368805),
				btVector3(20.000009536743164, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1377"),
                int(1)
			},

			{
				btVector3(-1418.4197998046875, 863.5012817382812,9.99596655368805),
				btVector3(20.000009536743164, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1376"),
                int(1)
			},

			{
				btVector3(-1418.4197998046875, 1299.4976806640625,9.99596655368805),
				btVector3(20.000009536743164, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1375"),
                int(1)
			},

			{
				btVector3(-1418.4197998046875, 1735.494384765625,9.99596655368805),
				btVector3(20.000009536743164, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1374"),
                int(1)
			},

			{
				btVector3(-1418.4197998046875, 2171.490478515625,9.99596655368805),
				btVector3(20.000009536743164, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1373"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1372"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1371"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1370"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1369"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1368"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1367"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1366"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1365"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1364"),
                int(1)
			},

			{
				btVector3(-1092.8141784667969, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1363"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1362"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1361"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1360"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1359"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1358"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1357"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1356"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1355"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1354"),
                int(1)
			},

			{
				btVector3(-767.2085571289062, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1353"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1352"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1351"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1350"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1349"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1348"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1347"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1346"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1345"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1344"),
                int(1)
			},

			{
				btVector3(-441.6028594970703, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1343"),
                int(1)
			},

			{
				btVector3(-115.997314453125, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1342"),
                int(1)
			},

			{
				btVector3(-115.997314453125, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1341"),
                int(1)
			},

			{
				btVector3(-115.997314453125, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1340"),
                int(1)
			},

			{
				btVector3(-115.997314453125, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1339"),
                int(1)
			},

			{
				btVector3(-115.997314453125, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1338"),
                int(1)
			},

			{
				btVector3(-115.997314453125, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1337"),
                int(1)
			},

			{
				btVector3(-115.997314453125, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1336"),
                int(1)
			},

			{
				btVector3(-115.997314453125, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1335"),
                int(1)
			},

			{
				btVector3(-115.997314453125, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1334"),
                int(1)
			},

			{
				btVector3(-115.997314453125, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1333"),
                int(1)
			},

			{
				btVector3(209.60830688476562, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1332"),
                int(1)
			},

			{
				btVector3(209.60830688476562, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1331"),
                int(1)
			},

			{
				btVector3(209.60830688476562, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1330"),
                int(1)
			},

			{
				btVector3(209.60830688476562, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1329"),
                int(1)
			},

			{
				btVector3(94.06496047973633, -8.4912109375,10.0),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1328"),
                int(1)
			},

			{
				btVector3(209.60830688476562, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1327"),
                int(1)
			},

			{
				btVector3(209.60830688476562, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1326"),
                int(1)
			},

			{
				btVector3(209.60830688476562, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1325"),
                int(1)
			},

			{
				btVector3(209.60830688476562, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1324"),
                int(1)
			},

			{
				btVector3(209.60830688476562, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1323"),
                int(1)
			},

			{
				btVector3(535.2140808105469, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1322"),
                int(1)
			},

			{
				btVector3(535.2140808105469, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1321"),
                int(1)
			},

			{
				btVector3(535.2140808105469, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1320"),
                int(1)
			},

			{
				btVector3(535.2140808105469, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1319"),
                int(0)
			},

			{
				btVector3(752.2522735595703, 374.6894073486328,294.09372329711914),
				btVector3(184.14979934692383, 184.14979934692383, 600.5661010742188),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1318"),
                int(1)
			},

			{
				btVector3(535.2140808105469, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1317"),
                int(1)
			},

			{
				btVector3(535.2140808105469, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1316"),
                int(1)
			},

			{
				btVector3(535.2140808105469, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1315"),
                int(1)
			},

			{
				btVector3(535.2140808105469, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1314"),
                int(1)
			},

			{
				btVector3(535.2140808105469, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1313"),
                int(1)
			},

			{
				btVector3(860.8195495605469, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1312"),
                int(1)
			},

			{
				btVector3(860.8195495605469, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1311"),
                int(1)
			},

			{
				btVector3(860.8195495605469, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1310"),
                int(1)
			},

			{
				btVector3(860.8195495605469, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1309"),
                int(1)
			},

			{
				btVector3(860.8195495605469, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1308"),
                int(1)
			},

			{
				btVector3(860.8195495605469, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1307"),
                int(1)
			},

			{
				btVector3(860.8195495605469, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1306"),
                int(1)
			},

			{
				btVector3(860.8195495605469, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1305"),
                int(1)
			},

			{
				btVector3(860.8195495605469, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1304"),
                int(1)
			},

			{
				btVector3(860.8195495605469, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1303"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1302"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1301"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1300"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1299"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1298"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, 427.5050354003906,7.228968143463135),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1297"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1296"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1295"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1294"),
                int(1)
			},

			{
				btVector3(1186.4251708984375, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1293"),
                int(1)
			},

			{
				btVector3(1512.0306396484375, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1292"),
                int(1)
			},

			{
				btVector3(1512.0306396484375, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1291"),
                int(1)
			},

			{
				btVector3(1512.0306396484375, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1290"),
                int(1)
			},

			{
				btVector3(1512.0306396484375, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1289"),
                int(1)
			},

			{
				btVector3(1512.0306396484375, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1288"),
                int(1)
			},

			{
				btVector3(1512.0306396484375, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1287"),
                int(1)
			},

			{
				btVector3(1512.0306396484375, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1286"),
                int(1)
			},

			{
				btVector3(1512.0306396484375, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1285"),
                int(1)
			},

			{
				btVector3(1512.0306396484375, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1284"),
                int(1)
			},

			{
				btVector3(746.1251831054688, -637.6168060302734,292.9811477661133),
				btVector3(184.14979934692383, 184.14979934692383, 600.5661010742188),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1283"),
                int(1)
			},

			{
				btVector3(-2704.2196655273438, 5.9132981300354,51.45334720611572),
				btVector3(5342.999267578125, 75.45332908630371, 198.79688262939453),
				btQuaternion(0.0,0.0,0.7071067690849304,-0.7071067690849304),
                std::string("Cube.1282"),
                int(1)
			},

			{
				btVector3(2612.587890625, -39.881746768951416,57.73476600646973),
				btVector3(5356.6680908203125, 75.45376300811768, 211.36653900146484),
				btQuaternion(0.0,0.0,0.7071067690849304,-0.7071067690849304),
                std::string("Cube.1281"),
                int(1)
			},

			{
				btVector3(-57.116546630859375, 2625.8404541015625,57.73476600646973),
				btVector3(5356.66748046875, 75.45375347137451, 211.36653900146484),
				btQuaternion(0.0,0.0,5.960464477539063e-08,-0.9999999403953552),
                std::string("Cube.1280"),
                int(1)
			},

			{
				btVector3(-102.9119873046875, -2690.1763916015625,51.45334720611572),
				btVector3(5342.998046875, 75.45331954956055, 198.79688262939453),
				btQuaternion(0.0,0.0,5.960464477539063e-08,-0.9999999403953552),
                std::string("Cube.1279"),
                int(1)
			} 

			};
			m_obstacles.insert(m_obstacles.end(), newObstacles.begin(), newObstacles.end());
		
			addBoxes(m_obstacles, LEVELOBSTACLETYPE);
		}
	