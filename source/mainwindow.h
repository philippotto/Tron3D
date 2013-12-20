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
		MainWindow (QWidget * parent = nullptr);
		virtual ~MainWindow();

	signals:
		void startGame(GameConfig config);

	public slots:
		void updatePlayerInputBoxes();

	private slots:
		void prepareGameStart();
		void splitscreenToggled();
		void fullscreenToggled();
		void bikeNumberChanged(int newBikeNumber);


	private:
		QStatusBar*		m_statusBar;
		QPushButton*	m_gameStartButton;
		QSpinBox*		m_bikeNumberSpinBox;
		QVector<QComboBox*>	m_playerComboBoxes;
		QCheckBox*		m_splitscreenCheckBox;
		QCheckBox*		m_fullscreenCheckBox;
		QCheckBox*		m_postProcessingCheckBox;
		QCheckBox*		m_debugViewCheckBox;



		TroenGame*	m_troenGame;
		QThread*	m_gameThread;

		double		m_lastTime;
		float		m_fps;
	};
}