#pragma once

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
}

namespace osgGA
{
	class CameraManipulator;
}

// Bullet
class btDiscreteDynamicsWorld;


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
}