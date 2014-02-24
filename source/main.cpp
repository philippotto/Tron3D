// STD
#include <iostream>
// Qt
#include <QApplication>
//troen
#include "MainWindow.h"

#define EXTERN
#include "globals.h"


#include <stdio.h>
#include "RakPeerInterface.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000



int main(int argc, char* argv[])
{

	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;

	printf("(C) or (S)erver?\n");
	gets(str);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else {
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);
		isServer = true;
	}


	// TODO - Add code body here

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;


	int result = -1;

	// register meta types
	qRegisterMetaType<GameConfig>("GameConfig");

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