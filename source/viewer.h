#pragma once

#include "forwarddeclarations.h"

#include <QtGui/QtGui>
#include <QWidget>

#include <osg/ref_ptr>


class Viewer : public QWidget
{
	public:
		Viewer();

	protected:
		virtual void paintEvent(QPaintEvent* event);
		osg::Camera * createCamera(int x, int y, int w, int h);

		osg::ref_ptr<osgViewer::Viewer> m_viewer;
		osg::ref_ptr<osg::Camera> m_camera;
		osg::ref_ptr<osg::Node> m_scene;

		QTimer m_timer;
};