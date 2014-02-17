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
			const input::BikeInputState::InputDevice& inputDevice,
			const btTransform initialPosition,
			const osg::Vec3 playerColor,
			const std::string playerName,
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

		// initialization & communication
		void attachTrackingCameras
			(osg::ref_ptr<NodeFollowCameraManipulator> &manipulator,
             std::shared_ptr<HUDController>& hudController);
        void attachTrackingCamera
            (osg::ref_ptr<NodeFollowCameraManipulator> &manipulator);
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);
		void attachGameView(osg::ref_ptr<osgViewer::View> gameView);
		void setPlayerNode(osg::Group* playerNode);
		osg::ref_ptr<osgViewer::View> getGameView();
		inline bool hasGameView() { return m_hasGameView; };

		// logic events
		void updateModel(const long double gameTime);
		void updateUniforms();
		void setState(const BIKESTATE newState, const double respawnTime = -1);
		void moveBikeToPosition(btTransform position);
		void activateTurbo();
		float getTurboInitiation();
		void registerCollision(btScalar impulse);
		void reset();
		// controlling the FenceController
		void removeAllFences();
		void setLimitFence(const bool boolean);
		int getFenceLimit();

		// getters & setters & attributes
		virtual osg::ref_ptr<osg::Group> getViewNode() override;
		osg::ref_ptr<input::Keyboard> getKeyboardHandler();
		bool hasKeyboardHandler();
		float getSpeed();
		float getHealth();
		float getPoints();
		BIKESTATE getState();
		double getRespawnTime();
		osg::Vec3 getPlayerColor() { return m_playerColor; };
		const std::string getPlayerName() { return m_playerName; };

		float increaseHealth(const float diff);
		float increasePoints(const float diff);
		int getDeathCount() { return m_deathCount; };
		int getKillCount() { return m_killCount; };
		void increaseDeathCount() { m_deathCount++; };
		void increaseKillCount() { m_killCount++; };

	private:
		// field of view methods
		void updateFov(const double speed);
		void setFovy(const float newFovy);
		float getFovy();
		float computeFovyDelta(const float speed, const float currentFovy);

		void initializeInput(const input::BikeInputState::InputDevice inputDevice);
		void setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState);
		
		// communication links
		osg::ref_ptr<osgViewer::View>		m_gameView;
		std::shared_ptr<FenceController>	m_fenceController;
		osg::ref_ptr<input::Keyboard>		m_keyboardHandler;
		std::shared_ptr<input::PollingDevice> m_pollingThread;
		osg::ref_ptr<input::BikeInputState> m_bikeInputState;

		bool m_hasGameView = false;
		// the following attributes only exist if the bikeController has a corresponding gameview
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

		// player attributes
		osg::Vec3	m_playerColor;
		std::string m_playerName;
		float	m_health;
		float	m_points;
		int		m_killCount;
		int		m_deathCount;
	};
}