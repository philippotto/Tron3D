#include "MainWindow.h"

#include <QApplication>

#include <iostream>
#include "physicsworld.h"

int main(int argc, char* argv[])
{

	//PhysicsWorld *world = new PhysicsWorld();
	//world->testBullet();
	
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


