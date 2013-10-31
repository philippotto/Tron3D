
#pragma once

#include <QMainWindow>

#include "viewerwidget.h"

class QGLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow(
		QWidget * parent = NULL
    ,   Qt::WindowFlags flags = NULL);

	virtual ~MainWindow();

protected:
    QGLWidget *glWidget;

private:
	osg::Camera * createCamera(int x, int y, int w, int h);

	osg::Camera * m_camera;
	osg::Node * m_scene;
	ViewerWidget * m_viewerWidget;
};