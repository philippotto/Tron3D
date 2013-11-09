#pragma once

#include <QMainWindow>

#include <osg/ref_ptr>

#include "forwarddeclarations.h"
#include "fmod_manager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow(
		QWidget * parent = NULL
    ,   Qt::WindowFlags flags = NULL);

	virtual ~MainWindow();

private:
	Viewer *m_viewer;
	cFMODManager *m_fmodManager;
};