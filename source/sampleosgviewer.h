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
	};
}

#ifdef WIN32
#include <osg/GL>
#include <windows.h>

/*! This file offers a customized osg::Viewer which can display a game view. V-Sync is turned off due to the adaptive game loop in TroenGame.cpp.*/
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
