#pragma once

// STD includes we need everywhere
#include <memory>
#include <iostream>

// This file contains forward declarations of classes

// Qt forward declarations

// OSG forward declarations
namespace osgViewer
{
	class Viewer;
	class CompositeViewer;
	class View;
	class StatsHandler;
}

namespace osgQt
{
	class GraphicsWindowQt;
}

namespace osg
{
	class Camera;
	class Node;
	class Group;
	class Geode;
	class GraphicsContext;
	class ShapeDrawable;
	class Vec3f;
	class Image;
}

namespace osgText
{
	class Text;
}


namespace osgGA
{
	class CameraManipulator;
	class NodeTrackerManipulator;
}

// Bullet
class btDiscreteDynamicsWorld;
class btSequentialImpulseConstraintSolver;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btRigidBody;
class btCollisionObject;
class btCollisionShape;
class btPersistentManifold;
class btVector3;
class btTransform;


// own classes
namespace troen
{
	class TroenGame;
	class TroenGameBuilder;
	class SampleOSGViewer;
	class GameEventHandler;
	class GameLogic;
	class GameConfig;
	class Player;

	class BikeModel;
	class LevelModel;
	class FenceModel;
	class AbstractModel;
	class PhysicsWorld;
	class ItemModel;

	class BikeController;
	class LevelController;
	class FenceController;
	class AbstractController;
	class HUDController;
	class ItemController;


	class BikeView;
	class LevelView;
	class FenceView;
	class AbstractView;
	class HUDView;
	class ItemView;
	class SkyDome;
	class PostProcessing;
	class NodeFollowCameraManipulator;
	class Reflection;
	class ResourcePool;

	class SplineDeformationRendering;


	namespace util
	{
		class ChronoTimer;
		class GLDebugDrawer;
	}
	namespace input
	{
		class Keyboard;
		class Gamepad;
		class BikeInputState;
		class PollingDevice;
		class AI;
		class RemotePlayer;
	}

	namespace sound
	{
		class AudioManager;
	}
	
	namespace networking
	{
		struct bikeUpdateMessage;
		class NetworkPlayerInfo;
		class NetworkManager;
		class ClientManager;
		class ServerManager;
	}
}