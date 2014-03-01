#include "bikecontroller.h"
// OSG
#include <osg/PositionAttitudeTransform>
#include <osgViewer/View>
//troen
#include "../constants.h"
#include "../view/bikeview.h"
#include "../view/nodefollowcameramanipulator.h"
#include "../model/bikemodel.h"
#include "../controller/fencecontroller.h"
#include "../controller/hudcontroller.h"
#include "../model/physicsworld.h"
#include "../sound/audiomanager.h"

#include "../input/keyboard.h"
#include "../input/gamepad.h"
#include "../input/gamepadps4.h"
#include "../input/ai.h"
#include "../input/pollingdevice.h"
#include "../globals.h"

#include "../resourcepool.h"

#include "../util/filteredrayresultcallback.h"

using namespace troen;

BikeController::BikeController(
	const input::BikeInputState::InputDevice& inputDevice,
	const btTransform initialTransform,
	const osg::Vec3 playerColor,
	const std::string playerName,
	ResourcePool *m_resourcePool,
	bool hasGameView) :
AbstractController(),
m_keyboardHandler(nullptr),
m_pollingThread(nullptr),
m_playerColor(playerColor),
m_initialTransform(initialTransform),
m_health(BIKE_DEFAULT_HEALTH),
m_points(0),
m_speed(0),
m_turboInitiated(false),
m_timeOfLastCollision(-1),
m_respawnTime(-1),
m_fenceLimitActivated(true),
m_state(BIKESTATE::WAITING),
m_hasGameView(hasGameView),
m_killCount(0),
m_deathCount(0),
m_playerName(playerName)
{
	m_view = std::make_shared<BikeView>(m_playerColor, m_resourcePool);

	m_fenceController = std::make_shared<FenceController>(this, m_playerColor, m_initialTransform);

	osg::ref_ptr<osg::Group> viewNode = std::static_pointer_cast<BikeView>(m_view)->getNode();
	m_model = std::make_shared<BikeModel>(m_initialTransform, viewNode, m_fenceController, this);

	initializeInput(inputDevice);
}

void BikeController::reset()
{
	if (m_pollingThread != nullptr)
		m_pollingThread->setVibration(false);

	removeAllFences();
	m_health = BIKE_DEFAULT_HEALTH;
	m_points = 0;
	m_timeOfLastCollision = -1;
}

void BikeController::registerCollision(btScalar impulse)
{
	if (impulse > 0) {
		m_timeOfLastCollision = g_gameTime;
	}
}

float BikeController::increaseHealth(float diff)
{
	m_health = clamp(0,BIKE_DEFAULT_HEALTH,m_health + diff);
	return m_health;
}

float BikeController::increasePoints(float diff)
{
	m_points += diff;
	return m_points;
}

BikeController::~BikeController()
{
	if (m_pollingThread != nullptr)
	{
		m_pollingThread->stop();
		m_pollingThread->wait();
	}
}

void BikeController::initializeInput(input::BikeInputState::InputDevice inputDevice)
{
	osg::ref_ptr<input::BikeInputState> bikeInputState = new input::BikeInputState();
	setInputState(bikeInputState);

	switch (inputDevice)
	{
	case input::BikeInputState::KEYBOARD_wasd:
	{
		m_keyboardHandler = new input::Keyboard(bikeInputState, std::vector<osgGA::GUIEventAdapter::KeySymbol>{
			osgGA::GUIEventAdapter::KEY_W,
				osgGA::GUIEventAdapter::KEY_A,
				osgGA::GUIEventAdapter::KEY_S,
				osgGA::GUIEventAdapter::KEY_D,
				osgGA::GUIEventAdapter::KEY_Space,
				osgGA::GUIEventAdapter::KEY_G
		});
		break;
	}
	case input::BikeInputState::KEYBOARD_arrows:
	{
		m_keyboardHandler = new input::Keyboard(bikeInputState, std::vector<osgGA::GUIEventAdapter::KeySymbol>{
			osgGA::GUIEventAdapter::KEY_Up,
				osgGA::GUIEventAdapter::KEY_Left,
				osgGA::GUIEventAdapter::KEY_Down,
				osgGA::GUIEventAdapter::KEY_Right,
				osgGA::GUIEventAdapter::KEY_Control_R,
				osgGA::GUIEventAdapter::KEY_M,
		});
		break;
	}
#ifdef WIN32
	case input::BikeInputState::GAMEPAD:
	{
		std::shared_ptr<input::Gamepad> gamepad = std::make_shared<input::Gamepad>(bikeInputState);

		if (gamepad->checkConnection())
		{
			std::cout << "[TroenGame::initializeInput] Gamepad connected on port " << gamepad->getPort() << std::endl;
		}
		else
		{
			std::cout << "[TroenGame::initializeInput] No gamepad connected!" << std::endl;
		}
		m_pollingThread = gamepad;
		m_pollingThread->start();
		break;
	}
#endif
	case input::BikeInputState::GAMEPADPS4:
	{
		std::shared_ptr<input::GamepadPS4> gamepad = std::make_shared<input::GamepadPS4>(bikeInputState);

		if (gamepad->checkConnection())
		{
			std::cout << "[TroenGame::initializeInput] PS4 Controller connected" << std::endl;
		}
		else
		{
			std::cout << "[TroenGame::initializeInput] No PS4 Controller connected!" << std::endl;
		}
		m_pollingThread = gamepad;
		m_pollingThread->start();
		break;
	}
	case input::BikeInputState::AI:
	{
		std::shared_ptr<input::AI> ai = std::make_shared<input::AI>(bikeInputState, this);
		m_pollingThread = ai;

		m_pollingThread->start();
		break;
	}
	default:
		break;
	}
}

osg::ref_ptr<input::Keyboard> BikeController::getKeyboardHandler()
{
	return m_keyboardHandler;
}

bool BikeController::hasKeyboardHandler()
{
	return m_keyboardHandler != nullptr;
}

void BikeController::setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
	std::static_pointer_cast<BikeModel>(m_model)->setInputState(bikeInputState);
}

void BikeController::attachTrackingCameras(
	osg::ref_ptr<NodeFollowCameraManipulator>& manipulator,
	std::shared_ptr<HUDController>& hudController)
{
	osg::ref_ptr<osg::Group> viewNode = std::static_pointer_cast<BikeView>(m_view)->getNode();
	osg::PositionAttitudeTransform* pat = dynamic_cast<osg::PositionAttitudeTransform*> (viewNode->getChild(0));

	// set the actual node as the track node, not the pat
	manipulator->setTrackNode(pat->getChild(0));
	if (hudController != nullptr)
		hudController->setTrackNode(pat->getChild(0));

	// set the bikeInputState
	manipulator->setBikeInputState(m_bikeInputState);


	// set camera position
	manipulator->setHomePosition(
		CAMERA_EYE_POSITION, // homeEye
		osg::Vec3f(), // homeCenter
		osg::Z_AXIS, // up
		false
		);
}
void BikeController::attachTrackingCamera(osg::ref_ptr<NodeFollowCameraManipulator>& manipulator)
{
	osg::ref_ptr<osg::Group> viewNode = std::static_pointer_cast<BikeView>(m_view)->getNode();
	osg::PositionAttitudeTransform* pat = dynamic_cast<osg::PositionAttitudeTransform*> (viewNode->getChild(0));

	// set the actual node as the track node, not the pat
	manipulator->setTrackNode(pat->getChild(0));

	// set camera position
	manipulator->setHomePosition(
		CAMERA_EYE_POSITION, // homeEye
		osg::Vec3f(), // homeCenter
		osg::Z_AXIS, // up
		false
		);
}

void BikeController::attachGameView(osg::ref_ptr<osgViewer::View> gameView)
{
	m_gameView = gameView;
}


void BikeController::setFovy(float newFovy)
{
	if (!m_gameView.valid()) return;
	double fovy, aspect, znear, zfar;
	m_gameView->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
	m_gameView->getCamera()->setProjectionMatrixAsPerspective(newFovy, aspect, znear, zfar);
}

float BikeController::getFovy()
{
	if (!m_gameView.valid())
		return 0;
	double fovy, aspect, znear, zfar;
	m_gameView->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
	return fovy;
}

float BikeController::computeFovyDelta(float speed, float currentFovy)
{
	long double timeFactor = getTimeFactor();

	m_speed = speed;

	float fovyFactor = (speed - BIKE_VELOCITY_MIN) / (BIKE_VELOCITY_MAX - BIKE_VELOCITY_MIN);
	fovyFactor = fovyFactor > 0 ? fovyFactor : 0;
	float newFovy = FOVY_INITIAL + interpolate(fovyFactor, InterpolateCubed) * FOVY_ADDITION_MAX;

	const float fovyDampening = 20.f;
	float fovyDelta = (newFovy - currentFovy) / fovyDampening * timeFactor;
	return clamp(-FOVY_DELTA_MAX, FOVY_DELTA_MAX, fovyDelta);
}

float BikeController::getSpeed()
{
	return m_speed;
}

float BikeController::getHealth()
{
	return m_health;
}

float BikeController::getPoints()
{
	return m_points;
}

void BikeController::activateTurbo()
{
	std::shared_ptr<BikeModel> bikeModel = std::static_pointer_cast<BikeModel>(m_model);

	float turboFactor = bikeModel->getTurboFactor();
	if (turboFactor <= 0)
		m_turboInitiated = true;
}

float BikeController::getTurboInitiation()
{
	return m_turboInitiated;
}

void BikeController::setState(BIKESTATE newState, double respawnTime /*=-1*/)
{
	if (m_state == newState)
		return;

	m_state = newState;
	m_respawnTime = respawnTime;
}

BikeController::BIKESTATE BikeController::getState()
{
	return m_state;
}

double BikeController::getRespawnTime()
{
	return m_respawnTime;
}

void BikeController::updateModel(const long double gameTime)
{
	switch (m_state)
	{
	case DRIVING:
	{
		double speed = std::static_pointer_cast<BikeModel>(m_model)->updateState(gameTime);
		increasePoints(speed / 1000);
		updateFov(speed);
		//std::cout << gameTime - (m_respawnTime + RESPAWN_DURATION) << ": DRIVING" << std::endl;
		break;
	}
	case RESPAWN:
	{
		std::static_pointer_cast<BikeModel>(m_model)->freeze();
		removeAllFencesFromModel();
		updateFov(0);
		//std::cout << gameTime - (m_respawnTime + RESPAWN_DURATION) << ": RESPAWN" << std::endl;
		if (gameTime > m_respawnTime + RESPAWN_DURATION * 2.f / 3.f)
		{
			//osg::Quat attitude = btToOSGQuat(m_initialTransform.getRotation());
			//std::static_pointer_cast<BikeView>(m_view)->m_pat->setAttitude(attitude);
			moveBikeToPosition(m_initialTransform);
			m_state = RESPAWN_PART_2;
		}
		break;
	}
	case RESPAWN_PART_2:
	{
		reset();
		updateFov(0);
		//std::cout << gameTime - (m_respawnTime + RESPAWN_DURATION) << ": RESPAWN_PART_2" << std::endl;
		if (gameTime > m_respawnTime + RESPAWN_DURATION)
		{
			//std::cout << gameTime - (m_respawnTime + RESPAWN_DURATION) << ": start Driving" << std::endl;
			m_state = DRIVING;
		}
		break;
	}
	case WAITING_FOR_GAMESTART:
	case WAITING:
		break;
	default:
		break;
	}

	increaseHealth(getTimeFactor() * 20.0);

	// turbo should be only applied in one frame
	if (m_turboInitiated)
		m_turboInitiated = false;

	if (m_pollingThread != nullptr)
	{
		m_pollingThread->setVibration(m_timeOfLastCollision != -1 && g_gameTime - m_timeOfLastCollision < VIBRATION_TIME_MS);
	}

	updateUniforms();
}

osg::ref_ptr<osg::Group> BikeController::getViewNode()
{
	osg::ref_ptr<osg::Group> group = new osg::Group();
	// TODO (dw) try not to disable culling, by resizing the childrens bounding boxes
	group->setCullingActive(false);
	group->addChild(m_fenceController->getViewNode());
	group->addChild(std::static_pointer_cast<BikeView>(m_view)->getNode());
	return group;
};

void BikeController::setPlayerNode(osg::Group* playerNode)
{
	// this is the node which holds the rootNode of the entire scene
	// it is used to expose player specific information to the shaders
	// this is only necessary if a gameView exists for this player

	m_playerNode = playerNode;
	m_timeOfCollisionUniform = new osg::Uniform("timeSinceLastHit", 100000.f);
	m_velocityUniform = new osg::Uniform("velocity", 0.f);
	m_timeFactorUniform = new osg::Uniform("timeFactor", 1.f);
	m_healthUniform = new osg::Uniform("healthNormalized", m_health / BIKE_DEFAULT_HEALTH);
	m_playerNode->getOrCreateStateSet()->addUniform(m_timeOfCollisionUniform);
	m_playerNode->getOrCreateStateSet()->addUniform(m_velocityUniform);
	m_playerNode->getOrCreateStateSet()->addUniform(m_timeFactorUniform);
	m_playerNode->getOrCreateStateSet()->addUniform(m_healthUniform);
}

void BikeController::attachWorld(std::shared_ptr<PhysicsWorld> &world) {

	m_world = world;

	world->addRigidBodies(getRigidBodies(), COLGROUP_BIKE, COLMASK_BIKE);
	m_fenceController->attachWorld(world);
}

void BikeController::removeAllFences()
{
	m_fenceController->removeAllFences();
}

void BikeController::removeAllFencesFromModel() {
	m_fenceController->removeAllFencesFromModel();
}

void BikeController::setLimitFence(bool boolean)
{
	m_fenceLimitActivated = boolean;
}

int BikeController::getFenceLimit() {
	if (m_fenceLimitActivated)
		return getPoints();
	else
		return 0;
}

long double BikeController::getTimeFactor()
{
	long double timeSinceLastUpdate = std::static_pointer_cast<BikeModel>(m_model)->getTimeSinceLastUpdate();
	return timeSinceLastUpdate / 16.7f;
}

void BikeController::moveBikeToPosition(btTransform transform)
{
	std::static_pointer_cast<BikeModel>(m_model)->moveBikeToPosition(transform);
	m_fenceController->setLastPosition(transform.getRotation(), transform.getOrigin());
}


osg::ref_ptr<osgViewer::View> BikeController::getGameView()
{
	return m_gameView;
};

void BikeController::updateUniforms()
{
	if (m_hasGameView) {
		m_timeOfCollisionUniform->set((float) g_gameTime - m_timeOfLastCollision);
		m_velocityUniform->set(std::static_pointer_cast<BikeModel>(m_model)->getVelocity());
		m_timeFactorUniform->set((float) getTimeFactor());
		m_healthUniform->set(m_health/BIKE_DEFAULT_HEALTH);
	}
}

void BikeController::updateFov(double speed)
{
	if (m_gameView.valid()) {
		float currentFovy = getFovy();
		setFovy(currentFovy + computeFovyDelta(speed, currentFovy));
	}
}


float BikeController::getDistanceToObstacle(double angle) {
	// angle specifies the deviation from the current direction the bike is heading
	
	float rayLength = 10000;

	if (m_world) {

		btDiscreteDynamicsWorld* discreteWorld = m_world->getDiscreteWorld();
		std::shared_ptr<BikeModel> bikeModel = std::static_pointer_cast<BikeModel>(m_model);

		btVector3 from, to, direction;
		direction = bikeModel->getDirection().rotate(btVector3(0, 0, 1), angle);
		from = bikeModel->getPositionBt();	
		to = from + direction * rayLength;
		
		FilteredRayResultCallback RayCallback(bikeModel->getRigidBody().get(), from, to);

		// Perform raycast
		discreteWorld->rayTest(from, to, RayCallback);

		if (RayCallback.hasHit()) {
			btVector3 collisionPoint;
			collisionPoint = RayCallback.m_hitPointWorld;
			
			return (collisionPoint - from).length();
		}
		else {
			return rayLength;
		}

	}

	return rayLength;
}