#pragma once

#include "forwardDeclarations.h"

#include <QtGui/QtGui>
#include <QWidget>

#include <osg/ref_ptr>


class ViewerWidget : public QWidget
{
	public:
		ViewerWidget(osg::Camera* camera, osg::Node* scene);

	protected:
		virtual void paintEvent(QPaintEvent* event);

		osg::ref_ptr<osgViewer::Viewer> m_viewer;
		QTimer m_timer;
};