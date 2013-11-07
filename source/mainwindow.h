#pragma once

#include <QMainWindow>

#include "forwarddeclarations.h"


namespace troen{

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow (QWidget * parent = NULL);
		virtual ~MainWindow();

	private:
		OSGWidget*	m_osgWidget;
		TroenGame*	m_troenGame;
		QThread*	m_gameThread;
	};


}