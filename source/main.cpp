#include "mainwindow.h"

#include <iostream>
#include <chrono>
#include <ctime>

#include <QApplication>

using namespace std;

int main(int argc, char* argv[])
{
    int result = -1;

    // setup application base settings
    QApplication::setApplicationName("Troen");
	QApplication * application = new QApplication(argc, argv);

    // create and open MainWindow (subclass of QMainWindow)
	MainWindow * mainWindow = new MainWindow();
    mainWindow->show();

	result = application->exec();

	// Clean Up
	delete mainWindow;
	delete application;

    return result;
}