#pragma once
// OSG
#include <osg/Referenced>
#include <osg/Camera>
#include <osgText/Text>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"


namespace troen
{

	class HUDView : public AbstractView
	{
	public:
		HUDView();
		void resize(int width, int height);
		void attachSceneToRadarCamera(osg::Group* scene);
		void setSpeedText(float speed);
		void setHealthText(float health);

	private:
		osg::Camera* createHUD();
		osg::Camera* createRadar();

		osg::ref_ptr<osg::Camera> m_camera;
		osg::ref_ptr<osg::Camera> m_radarCamera;
		osg::ref_ptr<osgText::Text> m_healthText;
		osg::ref_ptr<osgText::Text> m_speedText;

		osg::Geode* m_savedGeode;
	};
}