#pragma once
#include "btBulletDynamicsCommon.h"
#include <osg/PositionAttitudeTransform>


using namespace std;

class Conversion
{
public:
    static osg::Vec3 btToOSG(btVector3 vec3)
    {
        return osg::Vec3(vec3.x(), vec3.y(), vec3.z());
    }

	static osg::PositionAttitudeTransform *transformToPAT(btTransform transform)
    {
		osg::PositionAttitudeTransform *pat = new osg::PositionAttitudeTransform();
        osg::Vec3 pos = btToOSG(transform.getOrigin());
        osg::Quat rot = osg::Quat(transform.getRotation().getAngle(), btToOSG(transform.getRotation().getAxis()));
        pat->setPosition(pos);
        pat->setAttitude(rot);
		return pat;
    }

	static void  updateWithTransform(btTransform transform, osg::PositionAttitudeTransform *pat)
	{
		osg::Vec3 pos = btToOSG(transform.getOrigin());
		osg::Quat rot = osg::Quat(transform.getRotation().getAngle(), btToOSG(transform.getRotation().getAxis()));
		pat->setPosition(pos);
		pat->setAttitude(rot);
	}

	static osg::Matrix asOsgMatrix(const btTransform& t)
	{
			btScalar ogl[16];
			t.getOpenGLMatrix(ogl);
			osg::Matrix m(ogl);
			return m;
	}
};