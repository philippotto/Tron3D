#pragma once
// STD
#include <deque>
#include <random>
// OSG
#include <osgViewer/View>
#include <osgText/Text>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"
#include "../controller/hudcontroller.h"

namespace troen
{

	class HUDView : public AbstractView
	{
	public:
		HUDView(const int i, const std::vector<std::shared_ptr<Player>>& players);

		//
		// initialization
		//
		void attachSceneToRadarCamera(osg::Group* scene);
		void setTrackNode(osg::Node* trackNode);
		
		//
		// updating
		//
		void updateRadarCamera();
		void resize(const int width, const int height);
		void updateIngameMessageTexts(std::deque<std::shared_ptr<IngameMessage>>& messages);

		//
		// texts
		//
		void initializeText(
			osg::ref_ptr<osgText::Text> text,
			const osg::Vec3& position,
			const osg::Vec4& color,
			const osgText::Text::AlignmentType alignment,
			const int size);
		void setSpeedText(const float speed);
		void setHealthText(const float health);
		void setPointsText(const float points);
		void setCountdownText(const int countdown);
		void setCountdownText(const std::string text);
		void setTimeText(const double gameTime, const int timeLimit);
		void setKillCountText(const int i, const std::string& playerName, const int killCount);

	private:
		osg::ref_ptr<osg::Camera> createHUD(const std::vector<std::shared_ptr<Player>>& players);
		osg::ref_ptr<osg::Camera> createRadar(const int index);

		void resizeHudComponents(const int width, const int height);

		osg::ref_ptr<osg::Camera>	m_camera;
		osg::ref_ptr<osg::Camera>	m_radarCamera;
		osg::ref_ptr<osg::Geode>	m_savedGeode;
		osg::ref_ptr<osg::Node>		m_trackNode;
		osg::ref_ptr<osgGA::NodeTrackerManipulator> m_radarManipulator;

		osg::ref_ptr<osgText::Font> m_font;
		osg::ref_ptr<osgText::Text> m_healthText;
		osg::ref_ptr<osgText::Text> m_speedText;
		osg::ref_ptr<osgText::Text> m_pointsText;
		osg::ref_ptr<osgText::Text> m_countdownText;
		osg::ref_ptr<osgText::Text> m_timeText;
		osg::ref_ptr<osgText::Text> m_killCountTexts[6];
		osg::ref_ptr<osgText::Text> m_ingameMessageTexts[4];

		osg::Vec4 m_playerColor;
	};
}