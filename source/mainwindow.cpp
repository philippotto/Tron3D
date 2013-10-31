
#include "mainwindow.h"

#include <QtOpenGL>
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags)
:   QMainWindow(parent, flags),
    glWidget(NULL)
{
    // configure window
    setWindowTitle("Troen");
    
    // set window size
    QDesktopWidget desktopWidget;
    QRect displaySize = desktopWidget.screenGeometry();
    this->resize(displaySize.width()/3*2, displaySize.height()/3*2);
	
    // probably subclass the QGLWidget class for specific purposes
    glWidget = new QGLWidget;
    setCentralWidget(glWidget);
}

MainWindow::~MainWindow()
{
    
}

