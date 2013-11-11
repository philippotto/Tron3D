#pragma once

#include <memory>

// This file contains forward declarations of classes,
// so that header files need less includes

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
// namespace doesn't work? may the library was designed without it?

// namespace bt
// {
	class btDiscreteDynamicsWorld;
	class btSequentialImpulseConstraintSolver;
	class btDefaultCollisionConfiguration;
	class btCollisionDispatcher;
	class btBroadphaseInterface;
	class btRigidBody;
	class btCollisionShape;
// }


// forward declarations for own classes
namespace troen
{
	class OSGWidget;
	class TroenGame;
	class SampleOSGViewer;
	class PhysicsWorld;

	namespace util
	{
		class ChronoTimer;
	}

	namespace physics
	{
		class Bike;
		class Fence;
	}

	namespace input
	{
		class KeyboardEventHandler;
		class BikeInputState;
	}
}