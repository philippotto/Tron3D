#pragma once
// OSG
#include <osg/Geometry>
#include <osg/Geode>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"

namespace troen
{
	class FenceView : public AbstractView
	{
	public:
		FenceView(std::shared_ptr<FenceModel> &model);
		void addFencePart(osg::Vec3 lastPosition, osg::Vec3 currentPosition);
		void removeAllFences();

	private:
		void initializeFence();
		void initializeShader();
		osg::ref_ptr<osg::Geometry>		m_geometry;
		osg::ref_ptr<osg::Vec3Array>	m_coordinates;
		osg::ref_ptr<osg::DrawArrays>	m_drawArrays;
		osg::ref_ptr<osg::Geode>		m_geode;

		std::shared_ptr<FenceModel>		m_model;
		float m_fenceHeight;
	};
}