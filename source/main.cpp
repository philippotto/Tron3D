// STD
#include <iostream>
// Qt
#include <QApplication>
//troen
#include "MainWindow.h"

#define EXTERN
#include "globals.h"


#include <stdio.h>
#include <string.h>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID

#include "network\networkmanager.h"



using namespace troen;






int main(void)
{
	

	char str[512];
	NetworkManager networkManager;


	printf("(C) or (S)erver?\n");
	gets(str);

	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		networkManager.openClient();
	}
	else {
		networkManager.openServer();
	}




	//int result = -1;

	//// register meta types
	//qRegisterMetaType<GameConfig>("GameConfig");

	//// setup application settings
	//QApplication::setApplicationName("Troen");
	//QApplication * application = new QApplication(argc, argv);

	//troen::MainWindow * mainWindow = new troen::MainWindow();
	//mainWindow->show();

	//result = application->exec();

	//// Clean Up
	//delete mainWindow;
	//delete application;

	//return result;



}