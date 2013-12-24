#pragma once
// OSG
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"

namespace troen
{
	class FenceView : public AbstractView
	{
	public:
		FenceView(osg::Vec3 color, std::shared_ptr<AbstractModel>& model, int maxFenceParts);

		void addFencePart(osg::Vec3 lastPosition, osg::Vec3 currentPosition);
		void removeAllFences();
		void removeFirstFencePart();
		void enforceFencePartsLimit(int maxFenceParts);

		void updateFenceGap(osg::Vec3 lastPosition, osg::Vec3 position);

	private:
		void initializeFence();
		void initializeFenceGap();
		void initializeShader();
		osg::ref_ptr<osg::Geometry>		m_geometry;
		osg::ref_ptr<osg::Vec3Array>	m_coordinates;
		osg::ref_ptr<osg::DrawArrays>	m_drawArrays;
		osg::ref_ptr<osg::Geode>		m_geode;

		std::weak_ptr<FenceModel>		m_model;

		osg::Vec3 m_playerColor;
		float m_fenceHeight;
		int m_maxFenceParts;
	};
}