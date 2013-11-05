#pragma once

#include "forwarddeclarations.h"

#include <QtGui/QtGui>
#include <osgViewer/CompositeViewer>
#include <QWidget>
#include <osgQt/GraphicsWindowQt>

#include <osg/ref_ptr>


class Viewer : public QWidget,  public osgViewer::CompositeViewer
{
	public:
		Viewer();

	protected:
		virtual void paintEvent(QPaintEvent* event);
		osg::Camera * createCamera(osgQt::GraphicsWindowQt* gw, osgViewer::View* view);
		osgQt::GraphicsWindowQt* createGraphicsWindow(int x, int y, int w, int h);

		osg::ref_ptr<osgViewer::Viewer> m_viewer;
		osg::ref_ptr<osg::Camera> m_camera;
		osg::ref_ptr<osg::Node> m_scene;

		QTimer m_timer;
};