
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
				btVector3(-1582.1939086914062, -1218.8435363769531,29.70787525177002),
				btVector3(49.172000885009766, 1161.2409210205078, 80.09547233581543),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.026"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.1939086914062, -1923.8829040527344,19.02405023574829),
				btVector3(49.17172908782959, 82.94580459594727, 38.04810047149658),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.014"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.1939086914062, -2006.8287658691406,18.072850704193115),
				btVector3(49.17172908782959, 82.94580459594727, 36.14570140838623),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.013"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.1939086914062, -2089.774627685547,17.169208526611328),
				btVector3(49.17172908782959, 82.94580459594727, 34.338412284851074),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.012"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.1939086914062, -2172.720489501953,16.310746669769287),
				btVector3(49.17172908782959, 82.94580459594727, 32.621493339538574),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.011"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.1939086914062, -2255.6663513183594,15.495209693908691),
				btVector3(49.17172908782959, 82.94580459594727, 30.990419387817383),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.010"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.1939086914062, -2338.6122131347656,14.72044825553894),
				btVector3(49.17172908782959, 82.94580459594727, 29.44089651107788),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.009"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.193389892578, -1757.9911804199219,21.079280376434326),
				btVector3(49.17172431945801, 82.94578552246094, 42.15856075286865),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.008"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.193389892578, -1840.9370422363281,20.025315284729004),
				btVector3(49.17172431945801, 82.9457950592041, 40.05063056945801),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.007"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.193389892578, -1923.8829040527344,19.02405023574829),
				btVector3(49.17172431945801, 82.9457950592041, 38.04810047149658),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.006"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.193389892578, -2006.8287658691406,18.07284951210022),
				btVector3(49.17172431945801, 82.9457950592041, 36.14569902420044),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.004"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.193389892578, -2089.774627685547,17.169207334518433),
				btVector3(49.17172431945801, 82.9457950592041, 34.338414669036865),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.003"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.193389892578, -2172.720489501953,16.31074547767639),
				btVector3(49.17172431945801, 82.94578552246094, 32.62149095535278),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.002"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.193389892578, -2255.6663513183594,15.495210886001587),
				btVector3(49.17172431945801, 82.94578552246094, 30.990421772003174),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.001"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.193389892578, -2338.6122131347656,14.720449447631836),
				btVector3(49.17172431945801, 82.94578552246094, 29.440898895263672),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.000"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1849.4767761230469, -1840.9370422363281,20.025317668914795),
				btVector3(246.9929313659668, 82.94580459594727, 40.05063533782959),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.045"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.286376953125, 223.7447166442871,65.23354530334473),
				btVector3(45.41949272155762, 64.14011001586914, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.049"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1536.49169921875, 268.72106552124023,65.23354530334473),
				btVector3(45.41949272155762, 64.14011001586914, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.048"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1491.4956665039062, 319.03738021850586,65.23354530334473),
				btVector3(45.41949272155762, 64.14011001586914, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.047"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1465.5841064453125, 382.5472640991211,65.23354530334473),
				btVector3(45.41949272155762, 64.14011001586914, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.046"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1465.1974487304688, 569.344596862793,65.23354530334473),
				btVector3(45.4194974899292, 64.1401195526123, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.044"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1494.7972106933594, 634.0838623046875,65.23354530334473),
				btVector3(45.4194974899292, 64.1401195526123, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.043"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1539.7932434082031, 684.4001770019531,65.23354530334473),
				btVector3(45.4194974899292, 64.1401195526123, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.042"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.193389892578, -1675.0453186035156,22.188715934753418),
				btVector3(49.17172431945801, 82.94578552246094, 44.377431869506836),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1467"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.1939086914062, -1840.9370422363281,20.025315284729004),
				btVector3(49.17172908782959, 82.94580459594727, 40.05063056945801),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1458"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1724.2839050292969, 1710.9136962890625,38.14069986343384),
				btVector3(239.6636962890625, 82.94576644897461, 76.28139972686768),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.041"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2056.4901733398438, 1213.2389831542969,51.11218452453613),
				btVector3(104.74456787109375, 82.94576644897461, 102.22436904907227),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.040"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1656.1569213867188, -513.8046646118164,44.15262699127197),
				btVector3(96.58408164978027, 82.9457950592041, 88.30524444580078),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.039"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1988.0009460449219, -16.129989624023438,59.16874408721924),
				btVector3(246.60659790039062, 82.94585227966309, 118.33747863769531),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.038"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.1937561035156, -182.02133178710938,54.09463882446289),
				btVector3(49.172019958496094, 746.5116882324219, 130.46710014343262),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1446"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1582.1939086914062, -596.7501831054688,42.0501184463501),
				btVector3(49.171695709228516, 82.94578552246094, 84.1002368927002),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1441"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.1932373046875, -1094.4245910644531,31.96126699447632),
				btVector3(49.17196750640869, 1078.2949829101562, 84.1002368927002),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1424"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2136.1932373046875, -182.02138900756836,54.09464359283447),
				btVector3(49.172019958496094, 746.5118408203125, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1411"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1585.7339477539062, 1793.8592529296875,36.61337852478027),
				btVector3(49.171671867370605, 746.5117645263672, 88.30525398254395),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.033"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-1585.7177734375, 1048.8819885253906,56.43697261810303),
				btVector3(49.171857833862305, 723.2968139648438, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.024"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2132.8964233398438, 1172.9170227050781,52.84860610961914),
				btVector3(50.621323585510254, 972.1341705322266, 130.46709060668945),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.16"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2133.1736755371094, 2084.1696166992188,30.923802852630615),
				btVector3(49.171881675720215, 829.4576263427734, 76.28140449523926),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.5"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1388"),
                LEVELOBSTACLETYPE
			},

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
				btVector3(-767.2085571289062, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1362"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-767.2085571289062, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1361"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-767.2085571289062, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1360"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-767.2085571289062, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1359"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-767.2085571289062, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1358"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-767.2085571289062, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1356"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-767.2085571289062, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1355"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-767.2085571289062, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1354"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-767.2085571289062, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1353"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1352"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1351"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1350"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1349"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1348"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1347"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1346"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1345"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1344"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-441.6028594970703, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1343"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-115.997314453125, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1342"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-115.997314453125, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1341"),
                LEVELOBSTACLETYPE
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
				btVector3(-115.997314453125, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1334"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-115.997314453125, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1333"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(209.60830688476562, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1332"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(209.60830688476562, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1331"),
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
				btVector3(209.60830688476562, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1324"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(209.60830688476562, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1323"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(535.2140808105469, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1322"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(535.2140808105469, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1321"),
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
				btVector3(535.2140808105469, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1314"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(535.2140808105469, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1313"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(860.8195495605469, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1312"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(860.8195495605469, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1311"),
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
				btVector3(860.8195495605469, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1304"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(860.8195495605469, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1303"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1186.4251708984375, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1302"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1186.4251708984375, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1301"),
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
				btVector3(1186.4251708984375, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1294"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1186.4251708984375, 2171.490478515625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1293"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, -1752.4765014648438,9.99596655368805),
				btVector3(20.0, 20.000009536743164, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1292"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, -1316.4801025390625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1291"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, -880.4838562011719,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1290"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, -444.48760986328125,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1289"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, -8.4912109375,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1288"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, 427.5050354003906,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1287"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, 863.5012817382812,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1286"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, 1299.4976806640625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1285"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(1512.0306396484375, 1735.494384765625,9.99596655368805),
				btVector3(20.0, 20.0, 20.0),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1284"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(746.1251831054688, -637.6168060302734,292.9811477661133),
				btVector3(184.14979934692383, 184.14979934692383, 600.5661010742188),
				btQuaternion(0.0,0.0,0.0,1.0),
                std::string("Cube.1283"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(-2704.2196655273438, 5.9132981300354,51.45334720611572),
				btVector3(5342.999267578125, 75.45332908630371, 198.79688262939453),
				btQuaternion(0.0,0.0,0.7071067690849304,-0.7071067690849304),
                std::string("Cube.1282"),
                LEVELWALLTYPE
			},

			{
				btVector3(2612.587890625, -39.881746768951416,57.73476600646973),
				btVector3(5356.6680908203125, 75.45376300811768, 211.36653900146484),
				btQuaternion(0.0,0.0,0.7071067690849304,-0.7071067690849304),
                std::string("Cube.1281"),
                LEVELWALLTYPE
			},

			{
				btVector3(-57.116546630859375, 2625.8404541015625,57.73476600646973),
				btVector3(5356.66748046875, 75.45375347137451, 211.36653900146484),
				btQuaternion(0.0,0.0,5.960464477539063e-08,-0.9999999403953552),
                std::string("Cube.1280"),
                LEVELWALLTYPE
			},

			{
				btVector3(-102.9119873046875, -2690.1763916015625,51.45334720611572),
				btVector3(5342.998046875, 75.45331954956055, 198.79688262939453),
				btQuaternion(0.0,0.0,5.960464477539063e-08,-0.9999999403953552),
                std::string("Cube.1279"),
                LEVELWALLTYPE
			} 

			};
			m_obstacles.insert(m_obstacles.end(), newObstacles.begin(), newObstacles.end());
		
			addBoxes(m_obstacles);
		}
	