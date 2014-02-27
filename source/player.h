#pragma once
// STD
#include <memory>
// OSG
#include <osgViewer/View>
// troen
#include "forwarddeclarations.h"
#include "troengame.h"
#include "controller/bikecontroller.h"
#include "controller/bikecontroller.h"
#include "controller/fencecontroller.h"
#include "sampleosgviewer.h"

namespace troen
{
	class Player
	{
	public:
		Player(TroenGame* game,const std::shared_ptr<GameConfig>& config, const int id);
		void createHUDController(const std::vector<std::shared_ptr<Player>>& players);
		virtual ~Player();

		//
		// getter
		//
		const int getId() 			{ return m_id; };
		osg::Vec3 getPlayerColor()  { return m_playerColor; };
		const std::string getPlayerName() { return m_playerName; };
		int getDeathCount() 	{ return m_deathCount; };
		int getKillCount() 		{ return m_killCount; };
		const float getHealth() { return m_health; };
		const float getPoints()	{ return m_points; };
		//
		// controllers
		//
		std::shared_ptr<BikeController>	getBikeController()
			{ return m_bikeController; };
		std::shared_ptr<FenceController> getFenceController()
			{ return m_fenceController; };
		std::shared_ptr<HUDController>	getHUDController()
			{ return m_HUDController; };
		//
		// osg related
		//
		osg::ref_ptr<SampleOSGViewer> getViewer()	{ return m_viewer; };
		osg::ref_ptr<osgViewer::View> getGameView()	{ return m_gameView; };
		osg::ref_ptr<osg::Group> getPlayerNode()	{ return m_playerNode; };
		std::shared_ptr<Reflection> getReflection()	{ return m_reflection; };

		//
		// setter
		//
		void setHealth(float health)	{ m_health = health; };
		void setPoints(float points)	{ m_points = points; };

		//
		// logic methods
		//
		void increaseDeathCount() { m_deathCount++; };
		void increaseKillCount() { m_killCount++; };
		float increaseHealth(const float diff);
		float increasePoints(const float diff);


	private:
		int			m_id;
		//
		// player attributes
		//
		osg::Vec3	m_playerColor;
		std::string m_playerName;
		float		m_health;
		float		m_points;
		int			m_killCount;
		int			m_deathCount;

		//
		// controllers
		//
		std::shared_ptr<BikeController>	m_bikeController;
		std::shared_ptr<FenceController> m_fenceController;
		std::shared_ptr<HUDController>	m_HUDController;

		//
		// osg elements
		//
		osg::ref_ptr<SampleOSGViewer>	m_viewer;
		osg::ref_ptr<osgViewer::View>	m_gameView;
		osg::ref_ptr<osg::Group>		m_playerNode;
		std::shared_ptr<Reflection>		m_reflection;
	};
}