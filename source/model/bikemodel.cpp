#include "bikemodel.h"
// OSG
#include <osg/BoundingBox>
// STD
#include <cmath>
// troen
#include "../constants.h"
#include "../input/bikeinputstate.h"
#include "../controller/bikecontroller.h"
#include "bikemotionstate.h"
#include "objectinfo.h"

using namespace troen;

BikeModel::BikeModel(
	btTransform initialTransform,
	osg::ref_ptr<osg::Group> node,
	Player * player,
	BikeController* bikeController) :
AbstractModel(),
m_lastUpdateTime(0),
m_bikeController(bikeController)
{
	resetState();

	osg::BoundingBox bb;
	bb.expandBy(node->getBound());

	std::shared_ptr<btBoxShape> bikeShape = std::make_shared<btBoxShape>(BIKE_DIMENSIONS / 2);

	std::shared_ptr<BikeMotionState> bikeMotionState = std::make_shared<BikeMotionState>(
		initialTransform,
		dynamic_cast<osg::PositionAttitudeTransform*> (node->getChild(0)),
		player,
		this
	);

	// TODO
	// make friction & ineartia work without wrong behaviour of bike (left turn)
	btVector3 bikeInertia(0, 0, 0);
	bikeShape->calculateLocalInertia(BIKE_MASS, bikeInertia);
	//std::cout << bikeInertia.getX() << ".." << bikeInertia.getY() << ".." << bikeInertia.getZ() << std::endl;

	btRigidBody::btRigidBodyConstructionInfo m_bikeRigidBodyCI(BIKE_MASS, bikeMotionState.get(), bikeShape.get(), bikeInertia);
	m_bikeRigidBodyCI.m_friction = 0.f;
	//std::cout << m_bikeRigidBodyCI.m_friction << std::endl;

	std::shared_ptr<btRigidBody> bikeRigidBody = std::make_shared<btRigidBody>(m_bikeRigidBodyCI);

	bikeRigidBody->setCcdMotionThreshold(1 / BIKE_DIMENSIONS.y());
	bikeRigidBody->setCcdSweptSphereRadius(BIKE_DIMENSIONS.x() * .5f - BIKE_DIMENSIONS.x() * 0.01);
	// this seems to be necessary so that we can move the object via setVelocity()
	bikeRigidBody->setActivationState(DISABLE_DEACTIVATION);
	bikeRigidBody->setAngularFactor(btVector3(0, 0, 1));

	// for collision event handling
	ObjectInfo* info = new ObjectInfo(bikeController, BIKETYPE);
	bikeRigidBody->setUserPointer(info);

	bikeMotionState->setRigidBody(bikeRigidBody);

	m_collisionShapes.push_back(bikeShape);
	m_motionStates.push_back(bikeMotionState);
	m_rigidBodies.push_back(bikeRigidBody);
}

void BikeModel::setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
}

void BikeModel::resetState()
{
	m_oldVelocity = 0.0;
	m_rotation = 0.0;
	m_bikeFriction = 1.0;
}

float BikeModel::getSteering()
{
	return m_steering;
}


float BikeModel::getTurboFactor()
{
	// return value will either be between 0 and 1 or it is -1
	// it indicates if the "turbo phase" has just started or if it is already over or if it was ended abruptly
	// this can be used to compute the wheelyTilt of the bike
	return m_turboFactor;
}

void BikeModel::updateTurboFactor(float newVelocity, float time)
{
	m_turboFactor = max(0.f, m_turboFactor);

	if (m_bikeController->turboInitiated() || m_bikeInputState->getTurboPressed()) {
		m_turboFactor = 1.f;
		m_timeOfLastTurboInitiation = time;
	}
	else if (m_turboFactor > 0){
		if (m_oldVelocity - newVelocity > THRESHOLD_FOR_ABRUPT_VELOCITY_CHANGE) {
			// deactivate turbo phase if the bike speed was decreased abruptly (e.g. collision)
			m_turboFactor = -1.f;
		}
		else {
			m_turboFactor = 1 - (time - m_timeOfLastTurboInitiation) / TURBO_PHASE_LENGTH;
			m_turboFactor = max(0.f, m_turboFactor);
		}
	}

}

long double BikeModel::getTimeSinceLastUpdate()
{
	return m_timeSinceLastUpdate;
}

float BikeModel::updateState(long double time)
{
	m_timeSinceLastUpdate = time - m_lastUpdateTime;
	float timeFactor = m_timeSinceLastUpdate / 16.6f;

	m_lastUpdateTime = time;

	const btVector3 front = btVector3(0, -1, 0);

	// call this exactly once per frame
	m_steering = m_bikeInputState->getAngle();
	float acceleration = m_bikeInputState->getAcceleration();

	// if the handbrake is pulled, reduce friction to allow drifting
	m_bikeFriction = (abs(m_steering) > BIKE_ROTATION_VALUE) ? 0.03 * timeFactor : fmin((1.f + timeFactor * 0.13f) * m_bikeFriction, 1.0);

	std::shared_ptr<btRigidBody> bikeRigidBody = m_rigidBodies[0];

	btVector3 currentVelocityVectorXY = bikeRigidBody->getLinearVelocity();
	btScalar zComponent = currentVelocityVectorXY.getZ();
	currentVelocityVectorXY = btVector3(currentVelocityVectorXY.getX(), currentVelocityVectorXY.getY(), 0);
	btVector3 currentAngularVelocity = bikeRigidBody->getAngularVelocity();

	// accelerate
	float speedFactor = 1 - currentVelocityVectorXY.length() / BIKE_VELOCITY_MAX;
	// invsquared(t)   (1 - (1 - (t)) * (1 - (t)))
	float accInterpolation = acceleration * interpolate(speedFactor, InterpolateInvSquared);

	// TODO: merge turboInitiation and turboPressed (Philipp)
	float turboSpeed = 0;
	// only initiate turbo, if no other turbo is active
	if (getTurboFactor() == 0 && (m_bikeController->turboInitiated() || m_bikeInputState->getTurboPressed()))
	{
		turboSpeed =  BIKE_VELOCITY_MAX / 2;
	}

	float speed = currentVelocityVectorXY.length() + turboSpeed + ((accInterpolation * BIKE_ACCELERATION_FACTOR_MAX) - BIKE_VELOCITY_DAMPENING_TERM) * timeFactor;

	updateTurboFactor(speed, time);

	// rotate:
	// turnFactor
	// -> stronger steering for low velocities
	// -> weaker steering at high velocities
	float turnFactor = clamp(.1,1, BIKE_VELOCITY_MIN / (.5f * speed));
	float turningRad = PI / 180 * m_steering * (BIKE_TURN_FACTOR_MAX * turnFactor);

	bikeRigidBody->setAngularVelocity(btVector3(0, 0, turningRad));

	if (speed > BIKE_VELOCITY_MAX) {
		const float timeToSlowDown = 1000;
		// decrease speed so that the user will reach the maximum speed within timeToSlowDown milli seconds
		// this is done so that the turbo won't be resetted instantly
		speed -= (speed - BIKE_VELOCITY_MAX) * m_timeSinceLastUpdate / timeToSlowDown;
	}

	if (speed < BIKE_VELOCITY_MIN)
	 	speed = BIKE_VELOCITY_MIN;

	m_oldVelocity = speed;

	// adapt velocity vector to real direction
	float angle =  bikeRigidBody->getOrientation().getAngle();
	btVector3 axis = bikeRigidBody->getOrientation().getAxis();

	// restrict the drifting angle and increase friction if it gets too big
	if (abs(currentVelocityVectorXY.angle(front.rotate(axis, angle))) > PI_4) {
		m_bikeFriction = 0.1 * timeFactor;
	}

	// let the bike drift, if the friction is low
	currentVelocityVectorXY = ((1 - m_bikeFriction) * currentVelocityVectorXY + m_bikeFriction * front.rotate(axis, angle) * speed).normalized() * speed;
	currentVelocityVectorXY.setZ(zComponent);
	bikeRigidBody->setLinearVelocity(currentVelocityVectorXY);

	return speed;
}

float BikeModel::getRotation()
{
	return m_rotation;
}

float BikeModel::getVelocity()
{
	return m_oldVelocity;
}

osg::Vec3d BikeModel::getPositionOSG()
{
	btTransform trans;
	(m_rigidBodies[0]->getMotionState()->getWorldTransform(trans));

	return osg::Vec3d(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}

btVector3 BikeModel::getPositionBt()
{
	btTransform trans;
	(m_rigidBodies[0]->getMotionState()->getWorldTransform(trans));

	return trans.getOrigin();
}


void BikeModel::moveBikeToPosition(btTransform position)
{
	m_rigidBodies[0]->setWorldTransform(position);
	m_rigidBodies[0]->setAngularVelocity(btVector3(0, 0, 0));
	m_rigidBodies[0]->setLinearVelocity(btVector3(0, 0, 0));
}

void BikeModel::freeze()
{
	m_rigidBodies[0]->setAngularVelocity(btVector3(0, 0, 0));
	m_rigidBodies[0]->setLinearVelocity(btVector3(0, 0, 0));
}