#pragma once
// STD
#include <memory>
// OSG
#include <osgViewer/View>
// troen
#include "forwarddeclarations.h"
#include "troengame.h"
#include "sampleosgviewer.h"

#include "controller/bikecontroller.h"
#include "controller/bikecontroller.h"
#include "controller/fencecontroller.h"
#include "controller/hudcontroller.h"

namespace troen
{
	class Player
	{
	public:
		Player(
			TroenGame* game,
			const std::shared_ptr<GameConfig>& config, 
			const int id);
		void createHUDController(const std::vector<std::shared_ptr<Player>>& players);
		virtual ~Player();

		//
		// getters
		//
		const int id() 			{ return m_id; };
		osg::Vec3 color()		{ return m_color; };
		const std::string name(){ return m_name; };
		int deathCount() 		{ return m_deathCount; };
		int killCount() 		{ return m_killCount; };
		const float health()	{ return m_health; };
		const float points()	{ return m_points; };
		//
		// controllers
		//
		std::shared_ptr<BikeController>	bikeController()
			{ return m_bikeController; };
		std::shared_ptr<FenceController> fenceController()
			{ return m_fenceController; };
		std::shared_ptr<HUDController>	hudController()
			{ return m_HUDController; };
		//
		// osg related
		//
		osg::ref_ptr<SampleOSGViewer> viewer()		{ return m_viewer; };
		osg::ref_ptr<osgViewer::View> gameView()	{ return m_gameView; };
		osg::ref_ptr<osg::Group> playerNode()		{ return m_playerNode; };
		std::shared_ptr<Reflection> reflection()	{ return m_reflection; };

		//
		// setters
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
		//
		// player attributes
		//
		int			m_id;
		osg::Vec3	m_color;
		std::string m_name;
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