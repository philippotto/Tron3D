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
}

namespace osgGA
{
	class CameraManipulator;
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
	class SampleOSGViewer;
	class GameEventHandler;
	class GameLogic;

	class BikeModel;
	class LevelModel;
	class FenceModel;
	class AbstractModel;
	class PhysicsWorld;

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
	class SkyDome;
	class PostProcessing;
	class NodeFollowCameraManipulator;

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
	}

	namespace sound
	{
		class AudioManager;
	}
}