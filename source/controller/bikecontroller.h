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
			Player * player,
			const input::BikeInputState::InputDevice& inputDevice,
			const btTransform initialPosition,
			ResourcePool *resourcePool,
			const bool hasGameView);
		~BikeController();

		typedef enum enum_BIKESTATE
		{
			DRIVING,
			RESPAWN,
			RESPAWN_PART_2,
			WAITING_FOR_GAMESTART,
			WAITING
		} BIKESTATE;

		//
		// initialization & communication
		//
		void BikeController::attachTrackingCamera(std::shared_ptr<HUDController>& hudController);
        void attachTrackingCamera (osg::ref_ptr<NodeFollowCameraManipulator> &manipulator);
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);
		void attachGameView(osg::ref_ptr<osgViewer::View> gameView);

		void setPlayerNode(osg::Group* playerNode);
		bool hasGameView() { return m_hasGameView; };

		//
		// logic events
		//
		void updateModel(const long double gameTime);
		void updateUniforms();
		void setState(const BIKESTATE newState, const double respawnTime = -1);
		void moveBikeToPosition(btTransform position);
		void activateTurbo();
		void registerCollision(btScalar impulse);
		void reset();
		bool turboInitiated() { return m_turboInitiated; };
		
		//
		// getters & setters & attributes
		//
		virtual osg::ref_ptr<osg::Group> getViewNode() override;
		Player * player()		{ return m_player; };
		osg::ref_ptr<input::Keyboard> keyboardHandler()
								{ return m_keyboardHandler; };
		float speed()			{ return m_speed; };
		BIKESTATE state()		{ return m_state; };
		double respawnTime()	{ return m_respawnTime; };

		bool hasKeyboardHandler();


	private:
		// field of view methods
		void updateFov(const double speed);
		void setFovy(const float newFovy);
		float getFovy();
		float computeFovyDelta(const float speed, const float currentFovy);

		void initializeInput(const input::BikeInputState::InputDevice inputDevice);
		void setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState);
		long double getTimeFactor();
		
		// communication links
		Player * m_player;
		osg::ref_ptr<osgViewer::View>		m_gameView;
		osg::ref_ptr<input::Keyboard>		m_keyboardHandler;
		std::shared_ptr<input::PollingDevice> m_pollingThread;
		osg::ref_ptr<input::BikeInputState> m_bikeInputState;

		bool m_hasGameView = false;
		// the following attributes only exist if the bikeController
		// has a corresponding gameview
		osg::Uniform*	m_timeOfCollisionUniform;
		osg::Uniform*	m_velocityUniform;
		osg::Uniform*	m_timeFactorUniform;
		osg::Uniform*	m_healthUniform;
		osg::Group*		m_playerNode;

		// behaviour attributes
		btTransform	m_initialTransform;
		BIKESTATE	m_state;
		float		m_speed;
		bool		m_turboInitiated = false;
		float		m_timeOfLastCollision;
		double		m_respawnTime;
		bool		m_fenceLimitActivated;
	};
}