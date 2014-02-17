#pragma once
//Qt
#include <QWidget>
#include <QMainWindow>
#include <QStatusBar>
#include <QPushButton>
#include <QColorDialog>
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
		void startGame(const GameConfig config);

	public slots:
		void updatePlayerInputBoxes();

	protected:
		virtual bool eventFilter(QObject* object, QEvent* event);
		virtual void childEvent(QChildEvent* e);

	private slots:
		void prepareGameStart();
		void bikeNumberChanged(const int newBikeNumber);
		void chooseColor(const int i);

	private:
		void loadSettings();
		void saveSettings();
		QString			m_settingsFileName;
		QStatusBar*		m_statusBar;
		QPushButton*	m_gameStartButton;
		QSpinBox*		m_bikeNumberSpinBox;
		QVector<QComboBox*>	m_playerComboBoxes;
		QVector<QColor> m_playerColors;
		QVector<QPushButton*> m_colorButtons;
		QVector<QLineEdit*> m_playerNameLineEdits;
		QCheckBox*		m_fullscreenCheckBox;
		QCheckBox*		m_postProcessingCheckBox;
		QCheckBox*		m_testPerformanceCheckBox;
		QCheckBox*		m_debugViewCheckBox;
		QCheckBox*		m_reflectionCheckBox;
		std::vector<QCheckBox*> m_ownViewCheckboxes;
		QSpinBox*		m_timeLimitSpinBox;

		TroenGame*	m_troenGame;
		QThread*	m_gameThread;

		double		m_lastTime;
		float		m_fps;
	};
}