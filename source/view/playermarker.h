#pragma once
// OSG
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
// troen
#include "easemotion.h"
#include "../forwarddeclarations.h"
#include "abstractview.h"

namespace troen
{
	class PlayerMarker : public AbstractView
	{
	public:
		PlayerMarker(osg::Vec3 color);
		osg::ref_ptr<osg::Group> getNode();

	private:
		osg::Vec3 m_playerColor;
		osg::ref_ptr<osg::Group> m_node;
	};


	class FadeInOutCallback : public osg::NodeCallback
	{
		public:
			FadeInOutCallback(osg::Material* mat, osg::MatrixTransform *markerTransform);
			
			virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );
		
		protected:
			osg::ref_ptr<osgAnimation::InOutCubicMotion> _motion;
			osg::ref_ptr<osg::MatrixTransform> _markerTransform;
			osg::observer_ptr<osg::Material> _material;
			float _lastDistance;
			int _fadingState;
	};

}