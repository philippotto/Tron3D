#pragma once

// comment out to disable debug mode
#define DEBUG

// necessary for wireframe debugging ? but with this variable, compilation fails
// #define BT_USE_DOUBLE_PRECISION


// This file contains forward declarations of classes

// Qt forward declarations

// OSG forward declarations
namespace osgViewer
{
	class Viewer;
	class CompositeViewer;
	class View;
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
	class GraphicsContext;
	class ShapeDrawable;
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
class btCollisionShape;



// own classes
namespace troen
{
	class TroenGame;
	class SampleOSGViewer;

	class BikeModel;
	class LevelModel;
	class FenceModel;
	class AbstractModel;
	class PhysicsWorld;

	class BikeController;
	class LevelController;
	class FenceController;
	class AbstractController;

	class BikeView;
	class LevelView;
	class FenceView;
	class AbstractView;	

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
	}

	namespace sound
	{
		class AudioManager;
	}
}