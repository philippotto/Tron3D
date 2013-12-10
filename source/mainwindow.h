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
#include "troengame.h"


namespace troen
{
	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow (QWidget * parent = NULL);
		virtual ~MainWindow();

	signals:
		void startGame(GameConfig config);

	public slots:
		void updatePlayerInputBoxes();

	private:
		void loadSettings();
		void saveSettings();

	private slots:
		void prepareGameStart();

	private:
		QString			m_settingsFileName;
		QStatusBar*		m_statusBar;
		QPushButton*	m_gameStartButton;
		QSpinBox*		m_bikeNumberSpinBox;
		QVector<QComboBox*>	m_playerComboBoxes;
		QCheckBox*		m_splitscreenCheckBox;
		QCheckBox*		m_postProcessingCheckBox;

		TroenGame*	m_troenGame;
		QThread*	m_gameThread;

		double		m_lastTime;
		float		m_fps;
	};
}