// STD
#include <iostream>
// Qt
#include <QApplication>
//troen
#include "MainWindow.h"
#include "troengame.h"

#define EXTERN
#include "globals.h"

int main(int argc, char* argv[])
{
	int result = -1;

	// register meta types
	qRegisterMetaType<troen::GameConfig>("GameConfig");

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