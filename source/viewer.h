#pragma once

#include "forwarddeclarations.h"

#include <QtGui/QtGui>
#include <QWidget>

#include <osg/ref_ptr>


class Viewer : public QWidget
{
	public:
		Viewer(osg::Camera* camera, osg::Node* scene);

	protected:
		virtual void paintEvent(QPaintEvent* event);

		osg::ref_ptr<osgViewer::Viewer> m_viewer;
		QTimer m_timer;
};