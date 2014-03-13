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
		btVector3(136.69878005981445, 27.45476245880127,50.0),
		btVector3(120.1258373260498, 56.61898136138916, 1.1883634328842163),
		btQuaternion(0.0,0.0,-0.7071067690849304,0.7071068286895752),
            std::string("Cube.055"),
            LEVELGROUNDTYPE
	},

	{
		btVector3(453.3900833129883, 17.777156829833984,100.0),
		btVector3(203.03552627563477, 95.69684028625488, 2.00856015086174),
		btQuaternion(0.0,0.0,-0.7071067690849304,0.7071068286895752),
            std::string("Cube.054"),
            LEVELGROUNDTYPE
	},

	{
		btVector3(844.8583984375, 17.777156829833984,150.0),
		btVector3(372.92552947998047, 175.77117919921875, 3.6892232298851013),
		btQuaternion(0.0,0.0,-0.7071067690849304,0.7071068286895752),
            std::string("Cube.053"),
            LEVELGROUNDTYPE
	},

	{
		btVector3(1372.2879028320312, 8.099550008773804,200.0),
		btVector3(574.5083236694336, 269.7833061218262, 5.683411955833435),
		btQuaternion(0.0,0.0,-0.7071067690849304,0.7071068286895752),
            std::string("Cube.052"),
            LEVELGROUNDTYPE
	},

	{
		btVector3(1928.7503051757812, 8.099550008773804,267.76588439941406),
		btVector3(750.0635528564453, 353.52783203125, 7.420119047164917),
		btQuaternion(0.0,0.0,-0.7071067690849304,0.7071068286895752),
            std::string("Cube.051"),
            LEVELGROUNDTYPE
	},

	{
		btVector3(-660.911865234375, 84.43222999572754,19.976837635040283),
		btVector3(160.2307891845703, 139.69518661499023, 3.707292377948761),
		btQuaternion(-8.940696716308594e-08,-0.9845066070556641,0.17534896731376648,0.0),
            std::string("Cube.050"),
            LEVELGROUNDTYPE
	},

	{
		btVector3(1520.1722717285156, -2076.2135314941406,209.4871711730957),
		btVector3(49.171857833862305, 723.2968139648438, 130.46709060668945),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.037"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(-623.1343460083008, -1525.7380676269531,185.0114631652832),
		btVector3(49.17196750640869, 1078.2949829101562, 84.1002368927002),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.036"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(455.1601791381836, -1673.9303588867188,212.21893310546875),
		btVector3(246.60659790039062, 82.94585227966309, 118.33747863769531),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.035"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(-42.51445770263672, -2005.7743835449219,197.20281600952148),
		btVector3(96.58408164978027, 82.9457950592041, 88.30524444580078),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.034"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(1684.5292663574219, -1605.4411315917969,204.16236877441406),
		btVector3(104.74456787109375, 82.94576644897461, 102.22436904907227),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.032"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(2182.204132080078, -1937.6472473144531,191.19089126586914),
		btVector3(239.6636962890625, 82.94576644897461, 76.28139972686768),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.031"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(1155.6904602050781, -2122.1380615234375,218.28372955322266),
		btVector3(45.4194974899292, 64.1401195526123, 130.46709060668945),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.030"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(1105.3741455078125, -2167.1340942382812,218.28372955322266),
		btVector3(45.4194974899292, 64.1401195526123, 130.46709060668945),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.029"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(1040.6349182128906, -2196.733856201172,218.28372955322266),
		btVector3(45.4194974899292, 64.1401195526123, 130.46709060668945),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.028"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(853.8375854492188, -2196.347198486328,218.28372955322266),
		btVector3(45.41949272155762, 64.14011001586914, 130.46709060668945),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.027"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(790.3276062011719, -2170.4356384277344,218.28372955322266),
		btVector3(45.41949272155762, 64.14011001586914, 130.46709060668945),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.025"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(740.0112915039062, -2125.4396057128906,218.28372955322266),
		btVector3(45.41949272155762, 64.14011001586914, 130.46709060668945),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.023"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(695.0349426269531, -2079.6449279785156,218.28372955322266),
		btVector3(45.41949272155762, 64.14011001586914, 130.46709060668945),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.022"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(-1369.6467590332031, -1812.4546813964844,173.07550430297852),
		btVector3(246.9929313659668, 82.94580459594727, 40.05063533782959),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.021"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(-747.5532531738281, -2079.737548828125,182.75806427001953),
		btVector3(49.172000885009766, 1161.2409210205078, 80.09547233581543),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.020"),
            LEVELOBSTACLETYPE
	},

	{
		btVector3(423.8823699951172, 1425.8688354492188,267.76588439941406),
		btVector3(3498.9859008789062, 1649.1790771484375, 34.61426019668579),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.019"),
            LEVELGROUNDTYPE
	},

	{
		btVector3(-1009.3238067626953, 1133.8750457763672,19.976823329925537),
		btVector3(160.2307891845703, 139.6951961517334, 3.707292377948761),
		btQuaternion(-0.6961513161659241,-0.6961513161659241,0.12399046123027802,0.12399043887853622),
            std::string("Cube.018"),
            LEVELGROUNDTYPE
	},

	{
		btVector3(-1009.3238067626953, -133.32518577575684,19.976823329925537),
		btVector3(160.2307891845703, 139.6951961517334, 3.707292377948761),
		btQuaternion(-0.6961513161659241,-0.6961513161659241,0.12399046123027802,0.12399043887853622),
            std::string("Cube.015"),
            LEVELGROUNDTYPE
	},

	{
		btVector3(-408.31214904785156, -506.7597961425781,181.04969024658203),
		btVector3(160.23077011108398, 139.69521522521973, 3.7072929739952087),
		btQuaternion(0.9635283350944519,3.19306678875364e-08,-1.2020908712884193e-08,-0.2676074504852295),
            std::string("Cube.017"),
            LEVELGROUNDTYPE
	},

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
		btVector3(449.1987991333008, -1375.8157348632812,140.18000602722168),
		btVector3(3498.9859008789062, 1649.1790771484375, 39.48557138442993),
		btQuaternion(0.0,0.0,0.0,1.0),
            std::string("Cube.1384"),
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
