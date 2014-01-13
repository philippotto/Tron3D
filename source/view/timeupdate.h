#pragma once

class TimeUpdate : public osg::Uniform::Callback
{
public:
	virtual void operator()
		(osg::Uniform* uniform, osg::NodeVisitor* nv)
	{
		float time = nv->getFrameStamp()->getReferenceTime();
		uniform->set(time);
	}
};

