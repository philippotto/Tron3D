#pragma once
// OSG
#include <osgViewer/CompositeViewer>

namespace troen
{
	class SampleOSGViewer : public osgViewer::CompositeViewer
	{
	public:
		SampleOSGViewer(osg::GraphicsContext* context = nullptr);
		virtual ~SampleOSGViewer();

	private:
		void resize(int width, int height);
	};
}

#ifdef WIN32
#include <osg/GL>

#include <windows.h>

class RealizeOperation : public osg::GraphicsOperation
{
public:

	OpenThreads::Mutex  _mutex;
	std::string         _errorMessage;

	RealizeOperation() :
		osg::GraphicsOperation("RealizeOperation", false),
		_errorMessage() {}

	virtual void operator () (osg::GraphicsContext* gc)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);

		/* bind an extension */
		typedef BOOL(GL_APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (wglSwapIntervalEXT)
		{
			wglSwapIntervalEXT(0); //V-Sync off
		}
	}

};
#endif
