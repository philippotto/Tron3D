#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>


#include "forwarddeclarations.h"


namespace troen{

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow (QWidget * parent = NULL);
		virtual ~MainWindow();


	private slots:
		void showGame();

	private:
		QStackedWidget* m_stackedWidget;
		OSGWidget*	m_osgWidget;
		QPushButton* m_pushButton;


		TroenGame*	m_troenGame;

		QThread*	m_gameThread;
	};


}