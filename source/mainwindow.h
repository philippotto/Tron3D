#pragma once
//Qt
#include <QWidget>
#include <QMainWindow>
#include <QStatusBar>
#include <QStackedWidget>
#include <QPushButton>
// troen
#include "forwarddeclarations.h"


namespace troen
{
	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow (QWidget * parent = NULL);
		virtual ~MainWindow();

	public slots:
		void fpsChanged(double time);

	private:
		QStatusBar*		m_statusBar;
		QStackedWidget* m_stackedWidget;
		QPushButton*	m_pushButton;

		TroenGame*	m_troenGame;
		QThread*	m_gameThread;

		double		m_lastTime;
		float		m_fps;
	};
}