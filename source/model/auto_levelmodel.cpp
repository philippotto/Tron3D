
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
				btVector3(btScalar(-1582.1939086914062), btScalar(-1218.8435363769531), btScalar(29.70787525177002)),
				btVector3(btScalar(49.172000885009766), btScalar(1161.2409210205078), btScalar(80.09547233581543)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.026"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.1939086914062), btScalar(-1923.8829040527344), btScalar(19.02405023574829)),
				btVector3(btScalar(49.17172908782959), btScalar(82.94580459594727), btScalar(38.04810047149658)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.014"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.1939086914062), btScalar(-2006.8287658691406), btScalar(18.072850704193115)),
				btVector3(btScalar(49.17172908782959), btScalar(82.94580459594727), btScalar(36.14570140838623)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.013"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.1939086914062), btScalar(-2089.774627685547), btScalar(17.169208526611328)),
				btVector3(btScalar(49.17172908782959), btScalar(82.94580459594727), btScalar(34.338412284851074)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.012"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.1939086914062), btScalar(-2172.720489501953), btScalar(16.310746669769287)),
				btVector3(btScalar(49.17172908782959), btScalar(82.94580459594727), btScalar(32.621493339538574)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.011"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.1939086914062), btScalar(-2255.6663513183594), btScalar(15.495209693908691)),
				btVector3(btScalar(49.17172908782959), btScalar(82.94580459594727), btScalar(30.990419387817383)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.010"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.1939086914062), btScalar(-2338.6122131347656), btScalar(14.72044825553894)),
				btVector3(btScalar(49.17172908782959), btScalar(82.94580459594727), btScalar(29.44089651107788)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.009"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.193389892578), btScalar(-1757.9911804199219), btScalar(21.079280376434326)),
				btVector3(btScalar(49.17172431945801), btScalar(82.94578552246094), btScalar(42.15856075286865)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.008"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.193389892578), btScalar(-1840.9370422363281), btScalar(20.025315284729004)),
				btVector3(btScalar(49.17172431945801), btScalar(82.9457950592041), btScalar(40.05063056945801)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.007"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.193389892578), btScalar(-1923.8829040527344), btScalar(19.02405023574829)),
				btVector3(btScalar(49.17172431945801), btScalar(82.9457950592041), btScalar(38.04810047149658)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.006"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.193389892578), btScalar(-2006.8287658691406), btScalar(18.07284951210022)),
				btVector3(btScalar(49.17172431945801), btScalar(82.9457950592041), btScalar(36.14569902420044)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.004"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.193389892578), btScalar(-2089.774627685547), btScalar(17.169207334518433)),
				btVector3(btScalar(49.17172431945801), btScalar(82.9457950592041), btScalar(34.338414669036865)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.003"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.193389892578), btScalar(-2172.720489501953), btScalar(16.31074547767639)),
				btVector3(btScalar(49.17172431945801), btScalar(82.94578552246094), btScalar(32.62149095535278)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.002"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.193389892578), btScalar(-2255.6663513183594), btScalar(15.495210886001587)),
				btVector3(btScalar(49.17172431945801), btScalar(82.94578552246094), btScalar(30.990421772003174)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.001"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.193389892578), btScalar(-2338.6122131347656), btScalar(14.720449447631836)),
				btVector3(btScalar(49.17172431945801), btScalar(82.94578552246094), btScalar(29.440898895263672)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.000"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1849.4767761230469), btScalar(-1840.9370422363281), btScalar(20.025317668914795)),
				btVector3(btScalar(246.9929313659668), btScalar(82.94580459594727), btScalar(40.05063533782959)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.045"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.286376953125), btScalar(223.7447166442871), btScalar(65.23354530334473)),
				btVector3(btScalar(45.41949272155762), btScalar(64.14011001586914), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.049"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1536.49169921875), btScalar(268.72106552124023), btScalar(65.23354530334473)),
				btVector3(btScalar(45.41949272155762), btScalar(64.14011001586914), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.048"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1491.4956665039062), btScalar(319.03738021850586), btScalar(65.23354530334473)),
				btVector3(btScalar(45.41949272155762), btScalar(64.14011001586914), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.047"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1465.5841064453125), btScalar(382.5472640991211), btScalar(65.23354530334473)),
				btVector3(btScalar(45.41949272155762), btScalar(64.14011001586914), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.046"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1465.1974487304688), btScalar(569.344596862793), btScalar(65.23354530334473)),
				btVector3(btScalar(45.4194974899292), btScalar(64.1401195526123), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.044"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1494.7972106933594), btScalar(634.0838623046875), btScalar(65.23354530334473)),
				btVector3(btScalar(45.4194974899292), btScalar(64.1401195526123), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.043"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1539.7932434082031), btScalar(684.4001770019531), btScalar(65.23354530334473)),
				btVector3(btScalar(45.4194974899292), btScalar(64.1401195526123), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.042"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.193389892578), btScalar(-1675.0453186035156), btScalar(22.188715934753418)),
				btVector3(btScalar(49.17172431945801), btScalar(82.94578552246094), btScalar(44.377431869506836)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1467"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.1939086914062), btScalar(-1840.9370422363281), btScalar(20.025315284729004)),
				btVector3(btScalar(49.17172908782959), btScalar(82.94580459594727), btScalar(40.05063056945801)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1458"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1724.2839050292969), btScalar(1710.9136962890625), btScalar(38.14069986343384)),
				btVector3(btScalar(239.6636962890625), btScalar(82.94576644897461), btScalar(76.28139972686768)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.041"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2056.4901733398438), btScalar(1213.2389831542969), btScalar(51.11218452453613)),
				btVector3(btScalar(104.74456787109375), btScalar(82.94576644897461), btScalar(102.22436904907227)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.040"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1656.1569213867188), btScalar(-513.8046646118164), btScalar(44.15262699127197)),
				btVector3(btScalar(96.58408164978027), btScalar(82.9457950592041), btScalar(88.30524444580078)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.039"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1988.0009460449219), btScalar(-16.129989624023438), btScalar(59.16874408721924)),
				btVector3(btScalar(246.60659790039062), btScalar(82.94585227966309), btScalar(118.33747863769531)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.038"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.1937561035156), btScalar(-182.02133178710938), btScalar(54.09463882446289)),
				btVector3(btScalar(49.172019958496094), btScalar(746.5116882324219), btScalar(130.46710014343262)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1446"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1582.1939086914062), btScalar(-596.7501831054688), btScalar(42.0501184463501)),
				btVector3(btScalar(49.171695709228516), btScalar(82.94578552246094), btScalar(84.1002368927002)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1441"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.1932373046875), btScalar(-1094.4245910644531), btScalar(31.96126699447632)),
				btVector3(btScalar(49.17196750640869), btScalar(1078.2949829101562), btScalar(84.1002368927002)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1424"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2136.1932373046875), btScalar(-182.02138900756836), btScalar(54.09464359283447)),
				btVector3(btScalar(49.172019958496094), btScalar(746.5118408203125), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1411"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1585.7339477539062), btScalar(1793.8592529296875), btScalar(36.61337852478027)),
				btVector3(btScalar(49.171671867370605), btScalar(746.5117645263672), btScalar(88.30525398254395)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.033"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-1585.7177734375), btScalar(1048.8819885253906), btScalar(56.43697261810303)),
				btVector3(btScalar(49.171857833862305), btScalar(723.2968139648438), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.024"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2132.8964233398438), btScalar(1172.9170227050781), btScalar(52.84860610961914)),
				btVector3(btScalar(50.621323585510254), btScalar(972.1341705322266), btScalar(130.46709060668945)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.16"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2133.1736755371094), btScalar(2084.1696166992188), btScalar(30.923802852630615)),
				btVector3(btScalar(49.171881675720215), btScalar(829.4576263427734), btScalar(76.28140449523926)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.5"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(2171.490478515625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1388"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-10.268763303756714), btScalar(-301.5158462524414), btScalar(72.03860759735107)),
				btVector3(btScalar(529.6404647827148), btScalar(461.7601776123047), btScalar(12.254399061203003)),
				btQuaternion(btScalar(-4.367650063841211e-08), btScalar(-0.9845066666603088),0.17534898221492767, btScalar(-1.7463129831085666e-09)),
                std::string("Cube.1387"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(btScalar(-10.782455205917358), btScalar(-778.228759765625), btScalar(151.9006633758545)),
				btVector3(btScalar(531.1703491210938), btScalar(531.1703491210938), btScalar(8.43999981880188)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1386"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(btScalar(988.9772033691406), btScalar(162.55023956298828), btScalar(150.13311386108398)),
				btVector3(btScalar(288.5529136657715), btScalar(2405.4783630371094), btScalar(11.89049243927002)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1385"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(btScalar(568.7586212158203), btScalar(-879.2779541015625), btScalar(146.33319854736328)),
				btVector3(btScalar(683.0246734619141), btScalar(321.9303894042969), btScalar(19.53296184539795)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1384"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(btScalar(342.8825378417969), btScalar(918.4856414794922), btScalar(153.36544036865234)),
				btVector3(btScalar(1020.994873046875), btScalar(898.2110595703125), btScalar(6.920419335365295)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1383"),
                LEVELGROUNDTYPE
			},

			{
				btVector3(btScalar(-767.2085571289062), btScalar(-1752.4765014648438), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.000009536743164), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1362"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-767.2085571289062), btScalar(-1316.4801025390625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1361"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-767.2085571289062), btScalar(-880.4838562011719), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1360"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-767.2085571289062), btScalar(-444.48760986328125), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1359"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-767.2085571289062), btScalar(-8.4912109375), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1358"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-767.2085571289062), btScalar(863.5012817382812), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1356"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-767.2085571289062), btScalar(1299.4976806640625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1355"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-767.2085571289062), btScalar(1735.494384765625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1354"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-767.2085571289062), btScalar(2171.490478515625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1353"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(-1752.4765014648438), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.000009536743164), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1352"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(-1316.4801025390625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1351"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(-880.4838562011719), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1350"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(-444.48760986328125), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1349"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(-8.4912109375), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1348"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(427.5050354003906), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1347"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(863.5012817382812), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1346"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(1299.4976806640625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1345"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(1735.494384765625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1344"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-441.6028594970703), btScalar(2171.490478515625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1343"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(-1752.4765014648438), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.000009536743164), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1342"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(-1316.4801025390625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1341"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(-880.4838562011719), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1340"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(-444.48760986328125), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1339"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(-8.4912109375), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1338"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(427.5050354003906), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1337"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(863.5012817382812), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1336"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(1299.4976806640625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1335"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(1735.494384765625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1334"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-115.997314453125), btScalar(2171.490478515625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1333"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(209.60830688476562), btScalar(-1752.4765014648438), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.000009536743164), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1332"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(209.60830688476562), btScalar(-1316.4801025390625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1331"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(209.60830688476562), btScalar(-880.4838562011719), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1330"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(209.60830688476562), btScalar(-444.48760986328125), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1329"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(94.06496047973633), btScalar(-8.4912109375), btScalar(10.0)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1328"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(209.60830688476562), btScalar(427.5050354003906), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1327"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(209.60830688476562), btScalar(863.5012817382812), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1326"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(209.60830688476562), btScalar(1299.4976806640625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1325"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(209.60830688476562), btScalar(1735.494384765625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1324"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(209.60830688476562), btScalar(2171.490478515625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1323"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(535.2140808105469), btScalar(-1752.4765014648438), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.000009536743164), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1322"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(535.2140808105469), btScalar(-1316.4801025390625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1321"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(535.2140808105469), btScalar(-880.4838562011719), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1320"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(535.2140808105469), btScalar(-444.48760986328125), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1319"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(752.2522735595703), btScalar(374.6894073486328), btScalar(294.09372329711914)),
				btVector3(btScalar(184.14979934692383), btScalar(184.14979934692383), btScalar(600.5661010742188)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1318"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(535.2140808105469), btScalar(427.5050354003906), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1317"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(535.2140808105469), btScalar(863.5012817382812), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1316"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(535.2140808105469), btScalar(1299.4976806640625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1315"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(535.2140808105469), btScalar(1735.494384765625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1314"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(535.2140808105469), btScalar(2171.490478515625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1313"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(-1752.4765014648438), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.000009536743164), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1312"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(-1316.4801025390625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1311"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(-880.4838562011719), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1310"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(-444.48760986328125), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1309"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(-8.4912109375), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1308"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(427.5050354003906), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1307"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(863.5012817382812), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1306"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(1299.4976806640625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1305"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(1735.494384765625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1304"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(860.8195495605469), btScalar(2171.490478515625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1303"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(-1752.4765014648438), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.000009536743164), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1302"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(-1316.4801025390625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1301"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(-880.4838562011719), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1300"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(-444.48760986328125), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1299"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(-8.4912109375), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1298"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(427.5050354003906), btScalar(7.228968143463135)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1297"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(863.5012817382812), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1296"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(1299.4976806640625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1295"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(1735.494384765625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1294"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1186.4251708984375), btScalar(2171.490478515625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1293"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(-1752.4765014648438), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.000009536743164), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1292"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(-1316.4801025390625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1291"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(-880.4838562011719), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1290"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(-444.48760986328125), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1289"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(-8.4912109375), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1288"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(427.5050354003906), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1287"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(863.5012817382812), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1286"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(1299.4976806640625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1285"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(1512.0306396484375), btScalar(1735.494384765625), btScalar(9.99596655368805)),
				btVector3(btScalar(20.0), btScalar(20.0), btScalar(20.0)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1284"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(746.1251831054688), btScalar(-637.6168060302734), btScalar(292.9811477661133)),
				btVector3(btScalar(184.14979934692383), btScalar(184.14979934692383), btScalar(600.5661010742188)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.0, btScalar(1.0)),
                std::string("Cube.1283"),
                LEVELOBSTACLETYPE
			},

			{
				btVector3(btScalar(-2704.2196655273438), btScalar(5.9132981300354), btScalar(51.45334720611572)),
				btVector3(btScalar(5342.999267578125), btScalar(75.45332908630371), btScalar(198.79688262939453)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.7071067690849304, btScalar(-0.7071067690849304)),
                std::string("Cube.1282"),
                LEVELWALLTYPE
			},

			{
				btVector3(btScalar(2612.587890625), btScalar(-39.881746768951416), btScalar(57.73476600646973)),
				btVector3(btScalar(5356.6680908203125), btScalar(75.45376300811768), btScalar(211.36653900146484)),
				btQuaternion(btScalar(0.0), btScalar(0.0),0.7071067690849304, btScalar(-0.7071067690849304)),
                std::string("Cube.1281"),
                LEVELWALLTYPE
			},

			{
				btVector3(btScalar(-57.116546630859375), btScalar(2625.8404541015625), btScalar(57.73476600646973)),
				btVector3(btScalar(5356.66748046875), btScalar(75.45375347137451), btScalar(211.36653900146484)),
				btQuaternion(btScalar(0.0), btScalar(0.0),5.960464477539063e-08, btScalar(-0.9999999403953552)),
                std::string("Cube.1280"),
                LEVELWALLTYPE
			},

			{
				btVector3(btScalar(-102.9119873046875), btScalar(-2690.1763916015625), btScalar(51.45334720611572)),
				btVector3(btScalar(5342.998046875), btScalar(75.45331954956055), btScalar(198.79688262939453)),
				btQuaternion(btScalar(0.0), btScalar(0.0),5.960464477539063e-08, btScalar(-0.9999999403953552)),
                std::string("Cube.1279"),
                LEVELWALLTYPE
			}

			};
			m_obstacles.insert(m_obstacles.end(), newObstacles.begin(), newObstacles.end());

			addBoxes(m_obstacles);
		}
	