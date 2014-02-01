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
#include "../input/keyboard.h"

namespace troen
{
	class BikeController : public AbstractController
	{
	public:
		BikeController(
			input::BikeInputState::InputDevice inputDevice,
			btTransform initialPosition,
			osg::Vec3 playerColor,
			ResourcePool *resourcePool,
			bool hasGameView);
		~BikeController();

		typedef enum enum_BIKESTATE
		{
			DRIVING,
			RESPAWN,
			RESPAWN_PART_2,
			WAITING_FOR_GAMESTART,
			WAITING
		} BIKESTATE;

		void attachTrackingCameras
			(osg::ref_ptr<NodeFollowCameraManipulator> &manipulator,
             std::shared_ptr<HUDController>& hudController);
        void attachTrackingCamera
            (osg::ref_ptr<NodeFollowCameraManipulator> &manipulator);
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);
		void attachGameView(osg::ref_ptr<osgViewer::View> gameView);

		void setState(const BIKESTATE newState, const double respawnTime = -1);
		void updateModel(const long double gameTime);
		void updateUniforms();

		// getters
		virtual osg::ref_ptr<osg::Group> getViewNode() override;
		osg::ref_ptr<input::Keyboard> getKeyboardHandler();
		bool hasKeyboardHandler();
		float getSpeed();
		float getHealth();
		float getPoints();
		BIKESTATE getState();
		double getRespawnTime();

		// logic events
		void moveBikeToPosition(btTransform position);

		// controlling the FenceController
		void removeAllFences();
		void setLimitFence(const bool boolean);
		int getFenceLimit();

		void activateTurbo();
		float getTurboInitiation();

		float increaseHealth(const float diff);
		float increasePoints(const float diff);
		void registerCollision(btScalar impulse);
		void reset();
		void setPlayerNode(osg::Group* playerNode);
		bool hasGameView() { return m_hasGameView; };
		osg::ref_ptr<osgViewer::View> getGameView();

	private:
		void updateFov(const double speed);
		void setFovy(const float newFovy);
		float getFovy();
		float computeFovyDelta(const float speed, const float currentFovy);

		void initializeInput(const input::BikeInputState::InputDevice inputDevice);
		void setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState);
		
		std::shared_ptr<FenceController> m_fenceController;
		osg::ref_ptr<input::Keyboard> m_keyboardHandler;
		std::shared_ptr<input::PollingDevice> m_pollingThread;

		osg::ref_ptr<osgViewer::View> m_gameView;
		osg::Vec3 m_playerColor;
		btTransform m_initialTransform;

		float m_health;
		float m_points;
		float m_speed;
		bool m_turboInitiated = false;
		float m_timeOfLastCollision;
		double m_respawnTime;
		bool m_fenceLimitActivated;
		BIKESTATE m_state;
		
		
		bool m_hasGameView = false;
		// the following attributes only exist if the bikeController has a corresponding gameview
		osg::Uniform* m_timeOfCollisionUniform;
		osg::Group* m_playerNode;


	};
}