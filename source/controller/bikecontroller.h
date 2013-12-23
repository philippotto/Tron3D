#pragma once
// OSG
#include <osg/ref_ptr>
#include <osgGA/NodeTrackerManipulator>
//bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"
#include "../input/bikeinputstate.h"

namespace troen
{
	class BikeController : public AbstractController
	{
	public:
		BikeController(
			input::BikeInputState::InputDevice inputDevice,
			btTransform initialPosition);
		void attachTrackingCamera
			(osg::ref_ptr<NodeFollowCameraManipulator> &manipulator);
		void attachWorld(std::weak_ptr<PhysicsWorld> &world);
		void attachGameView(osg::ref_ptr<osgViewer::View> gameView);

		void updateModel(long double time);

		// getters
		virtual osg::ref_ptr<osg::Group> getViewNode() override;
		osg::ref_ptr<input::Keyboard> getEventHandler();
		bool hasEventHandler();

		// logic events
		void moveBikeToPosition(btTransform position);

		// controlling the FenceController
		void removeAllFences();
		void enforceFencePartsLimit(int maxFenceParts);

	private:
		osg::Vec3 generateRandomColor();

		void setFovy(float newFovy);
		float getFovy();
		float computeFovyDelta(float speed, float currentFovy);

		void initializeInput(input::BikeInputState::InputDevice inputDevice);
		void setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState);

		std::shared_ptr<FenceController> m_fenceController;
		osg::ref_ptr<input::Keyboard> m_keyboardHandler = nullptr;
		osg::ref_ptr<osgViewer::View> m_gameView;

		osg::Vec3 m_playerColor;
		btTransform m_initialTransform;
	};
}