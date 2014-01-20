#pragma once

#include <osgUtil/CullVisitor>

class NearFarCallback : public osg::NodeCallback
{
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		traverse(node, nv);

		osgUtil::CullVisitor * cv = dynamic_cast<osgUtil::CullVisitor*> (nv);
		if (cv)
		{
			double n = cv->getCalculatedNearPlane();
			double f = cv->getCalculatedFarPlane();

			osg::Matrixd m = *cv->getProjectionMatrix();
			cv->clampProjectionMatrix(m, n, f);

			if (n != m_oldNear || f != m_oldFar)
			{
				m_oldNear = n;
				m_oldFar = f;
				g_nearFarUniform->set(osg::Vec2(n, f));
			}
		}
	}

private:
	double m_oldNear;
	double m_oldFar;
};