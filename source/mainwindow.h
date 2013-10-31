
#pragma once

#include <QMainWindow>

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
};