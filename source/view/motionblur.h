#pragma once

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <iostream>

namespace troen
{
	class MotionBlurOperation : public osg::Operation
	{
	public:
		MotionBlurOperation(double persistence) :
			osg::Operation("MotionBlur", true),
			cleared_(false),
			persistence_(persistence)
		{
			}

		virtual void operator () (osg::Object* object)
		{
			osg::GraphicsContext* gc = dynamic_cast<osg::GraphicsContext*>(object);
			if (!gc) return;

			double t = gc->getState()->getFrameStamp()->getSimulationTime();

			if (!cleared_)
			{
				// clear the accumulation buffer
				glClearColor(0, 0, 0, 0);
				glClear(GL_ACCUM_BUFFER_BIT);
				cleared_ = true;
				t0_ = t;
			}

			double dt = fabs(t - t0_);
			t0_ = t;

			// compute the blur factor
			double s = powf(0.2, dt / persistence_);

			// scale, accumulate and return
			glAccum(GL_MULT, s);
			glAccum(GL_ACCUM, 1 - s);
			glAccum(GL_RETURN, 1.0f);
		}

	private:
		bool cleared_;
		double t0_;
		double persistence_;
	};
}