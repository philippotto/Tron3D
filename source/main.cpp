// STD
#include <iostream>
// Qt
#include <QApplication>
//troen
#include "MainWindow.h"
#include "troengame.h"

#define EXTERN
#include "globals.h"


#include <stdio.h>
#include <string.h>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID

using namespace troen;


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