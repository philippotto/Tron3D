#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>

#include <memory>


#include "forwarddeclarations.h"


namespace troen{

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow (QWidget * parent = NULL);
		virtual ~MainWindow();

	private:
		QStackedWidget* m_stackedWidget;
		QPushButton* m_pushButton;

		TroenGame*	m_troenGame;
		QThread*	m_gameThread;
	};


}