#pragma once
// OSG
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <osg/Shader>
#include <osg/Program>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"
#include "../resourcepool.h"


namespace troen
{

	class BikeView : public AbstractView
	{
	public:
		BikeView(osg::Vec3 color, ResourcePool* resourcePool);
		void setTexture(osg::ref_ptr<osg::StateSet> stateset, ResourcePool::TextureResource textureName, int unit);

		osg::ref_ptr<osg::Node> createCyclePart(
			ResourcePool::ModelResource objName,
			ResourcePool::TextureResource specularTexturePath,
			ResourcePool::TextureResource diffuseTexturePath,
			ResourcePool::TextureResource normalTexturePath,
			int modelIndex,
			float glowIntensity = 1.f);

		osg::ref_ptr<osg::PositionAttitudeTransform> m_pat;
		osg::ref_ptr<osg::Node> m_MovieCycle_Body;

		void update();
		void createPlayerMarker(osg::Vec3 color);

	private:
		osg::Vec3 m_playerColor;
		osg::ref_ptr<osg::Node> m_playermarkerNode;
		ResourcePool* m_resourcePool;
	};
}