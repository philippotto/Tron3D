#pragma once

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
	class GameLogic;

	class BikeModel;
	class LevelModel;
	class FenceModel;
	class AbstractModel;

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
	}
	namespace input
	{
		class Keyboard;
		class Gamepad;
		class BikeInputState;
	}
}