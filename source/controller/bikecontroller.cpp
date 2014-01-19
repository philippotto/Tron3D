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
#include "../model/physicsworld.h"
#include "../sound/audiomanager.h"

#include "../input/keyboard.h"
#include "../input/gamepad.h"
#include "../input/gamepadps4.h"
#include "../input/ai.h"
#include "../input/pollingdevice.h"
#include "../globals.h"

using namespace troen;



BikeController::BikeController(
	input::BikeInputState::InputDevice inputDevice,
	btTransform initialTransform,
	osg::Vec3 playerColor) :
m_initialTransform(initialTransform)
{
	AbstractController();
	m_playerColor = playerColor;

	m_health = BIKE_DEFAULT_HEALTH;
	m_timeOfLastCollision = -1;

	m_view = std::make_shared<BikeView>(m_playerColor);
	m_fenceController = std::make_shared<FenceController>(m_playerColor,m_initialTransform);

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
	m_timeOfLastCollision = -1;
}

void BikeController::registerCollision(btScalar impulse)
{
	if (impulse > 0)
		m_timeOfLastCollision = g_currentTime;
}

float BikeController::increaseHealth(float diff)
{
	m_health += diff;
	return m_health;
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
		std::shared_ptr<input::AI> ai = std::make_shared<input::AI>(bikeInputState);
		m_pollingThread = ai;
		m_pollingThread->start();
		break;
	}
    default:
        break;
	}
}

osg::ref_ptr<input::Keyboard> BikeController::getEventHandler()
{
	return m_keyboardHandler;
}

bool BikeController::hasEventHandler()
{
	return m_keyboardHandler != nullptr;
}

void BikeController::setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState)
{
	std::static_pointer_cast<BikeModel>(m_model)->setInputState(bikeInputState);
}

void BikeController::attachTrackingCamera
  (osg::ref_ptr<NodeFollowCameraManipulator>& manipulator)
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
	long double timeSinceLastUpdate = std::static_pointer_cast<BikeModel>(m_model)->getTimeSinceLastUpdate();
	long double timeFactor = timeSinceLastUpdate / 16.7f;
	

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

void BikeController::activateTurbo()
{
	m_turboInitiated = true;
}

float BikeController::getTurboInitiation()
{
	return m_turboInitiated;
}

void BikeController::updateModel(long double time)
{
	double speed = std::static_pointer_cast<BikeModel>(m_model)->updateState(time);
	
	// turbo should be only applied in one frame
	if (m_turboInitiated)
		m_turboInitiated = false;

	if (m_pollingThread != nullptr)
	{
		m_pollingThread->setVibration(m_timeOfLastCollision != -1 && g_currentTime - m_timeOfLastCollision < VIBRATION_TIME_MS);
	}

	
	if (m_gameView.valid()) {
		float currentFovy = getFovy();
		setFovy(currentFovy + computeFovyDelta(speed, currentFovy));
	}
	
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

void BikeController::attachWorld(std::shared_ptr<PhysicsWorld> &world) {
	world->addRigidBodies(getRigidBodies(),COLGROUP_BIKE, COLMASK_BIKE);
	m_fenceController->attachWorld(world);
}

void BikeController::removeAllFences()
{
	m_fenceController->removeAllFences();
}

void BikeController::enforceFencePartsLimit(int maxFenceParts)
{
	m_fenceController->enforceFencePartsLimit(maxFenceParts);
}

void BikeController::moveBikeToPosition(btTransform transform)
{
	std::static_pointer_cast<BikeModel>(m_model)->moveBikeToPosition(transform);
	m_fenceController->setLastPosition(transform.getRotation(), transform.getOrigin());
}
