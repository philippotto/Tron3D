#pragma once
//Qt
#include <QWidget>
#include <QMainWindow>
#include <QStatusBar>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
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

	signals:
		void startGame();

	public slots:
		void updatePlayerInputBoxes();

	private slots:
		void prepareGameStart();


	private:
		QStatusBar*		m_statusBar;
		QPushButton*	m_gameStartButton;
		QSpinBox*		m_bikeNumberSpinBox;
		QVector<QComboBox*>	m_playerComboBoxes;
		QCheckBox*		m_splitscreenCheckBox;

		TroenGame*	m_troenGame;
		QThread*	m_gameThread;

		double		m_lastTime;
		float		m_fps;
	};
}