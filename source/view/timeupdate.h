#pragma once

/*! This class is a Uniform::Callback which is used to provide a time variable to the shaders.*/
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

