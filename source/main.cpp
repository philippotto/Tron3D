#include "MainWindow.h"

#include <QApplication>

#include <iostream>
#include "physics/physicsworld.h"

int main(int argc, char* argv[])
{
	int result = -1;

	// setup application settings
	QApplication::setApplicationName("Troen");
	QApplication * application = new QApplication(argc, argv);

	troen::MainWindow * mainWindow = new troen::MainWindow();
	mainWindow->show();

	result = application->exec();

	// Clean Up
	delete mainWindow;
	delete application;

	return result;
}


