#include "bikecontroller.h"
// OSG
#include <osg/PositionAttitudeTransform>
//troen
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

using namespace troen;

BikeController::BikeController(input::BikeInputState::InputDevice inputDevice, btTransform initialTransform) :
m_initialTransform(initialTransform)
{
	// TODO change random generation of player color here (and remove generateRandomColor-Function)
	m_playerColor = generateRandomColor();

	m_view = std::make_shared<BikeView>(m_playerColor);
	m_fenceController = std::make_shared<FenceController>(m_playerColor);

	osg::ref_ptr<osg::Group> viewNode = std::static_pointer_cast<BikeView>(m_view)->getNode();
	m_model = std::make_shared<BikeModel>(m_initialTransform, viewNode, m_fenceController, this);

	initializeInput(inputDevice);
}


osg::Vec3 BikeController::generateRandomColor() {
	// initialize rand
	srand(time(NULL));

	int r, g, b;
	r = rand() > RAND_MAX / 2 ? 1 : 0;
	g = rand() > RAND_MAX / 2 ? 1 : 0;

	if (r + g == 0)
		b = 1.f;
	else
		b = rand() > RAND_MAX / 2 ? 1 : 0;

	return osg::Vec3(r, g, b);
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
				osgGA::GUIEventAdapter::KEY_D
			});
		break;
	}
	case input::BikeInputState::KEYBOARD_arrows:
	{
		m_keyboardHandler = new input::Keyboard(bikeInputState, std::vector<osgGA::GUIEventAdapter::KeySymbol>{
			osgGA::GUIEventAdapter::KEY_Up,
			osgGA::GUIEventAdapter::KEY_Left,
			osgGA::GUIEventAdapter::KEY_Down,
			osgGA::GUIEventAdapter::KEY_Right
		});
		break;
	}	
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
		bikeInputState->setPollingDevice(gamepad);
		break;
	}
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
		bikeInputState->setPollingDevice(gamepad);
		break;
	}
	case input::BikeInputState::AI:
	{
		std::shared_ptr<input::AI> ai = std::make_shared<input::AI>(bikeInputState);
		bikeInputState->setPollingDevice(ai);
		break;
	}
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
//  (osg::ref_ptr<osgGA::NodeTrackerManipulator>& manipulator)
  (osg::ref_ptr<NodeFollowCameraManipulator>& manipulator)
{
	int debugNormalizer = 1;
#ifdef _DEBUG
	debugNormalizer = -1;
#endif

	osg::ref_ptr<osg::Group> viewNode = std::static_pointer_cast<BikeView>(m_view)->getNode();
	osg::PositionAttitudeTransform* pat = dynamic_cast<osg::PositionAttitudeTransform*> (viewNode->getChild(0));

	// set the actual node as the track node, not the pat
	manipulator->setTrackNode(pat->getChild(0));

	// set camera position
	manipulator->setHomePosition(
		osg::Vec3f(0.f, debugNormalizer * -135.f, 20.f), // homeEye
		osg::Vec3f(), // homeCenter
		osg::Z_AXIS, // up
		false
		);
}

void BikeController::updateModel()
{
	std::static_pointer_cast<BikeModel>(m_model)->updateState();

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

void BikeController::attachWorld(std::weak_ptr<PhysicsWorld> &world) {
	world.lock()->addRigidBodies(getRigidBodies());
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
