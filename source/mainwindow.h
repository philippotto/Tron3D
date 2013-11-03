#pragma once

#include <QMainWindow>

#include <osg/ref_ptr>

#include "forwardDeclarations.h"
#include "Fmod_manager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow(
		QWidget * parent = NULL
    ,   Qt::WindowFlags flags = NULL);

	virtual ~MainWindow();

private:
	osg::Camera * createCamera(int x, int y, int w, int h);

	osg::ref_ptr<osg::Camera> m_camera;
	osg::ref_ptr<osg::Node> m_scene;
	ViewerWidget *m_viewerWidget;
	cFMODManager *m_fmodManager;
};