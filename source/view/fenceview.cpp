#include "fenceview.h"
// OSG
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Vec4>
#include <osg/PositionAttitudeTransform>
// troen
#include "shaders.h"
#include "../model/fencemodel.h"

using namespace troen;

FenceView::FenceView(osg::Vec3 color, std::shared_ptr<FenceModel>& model, int maxFenceParts) : m_maxFenceParts(maxFenceParts)
{
	m_playerColor = color;
	m_model = model;
	m_node = new osg::Group();

	initializeFence();
	initializeShader();
}

void FenceView::initializeFence()
{
	m_fenceHeight = m_model.lock()->getFenceHeight() - 3;

	m_coordinates = new osg::Vec3Array;
	m_coordinates->setDataVariance(osg::Object::DYNAMIC);

	m_geometry = new osg::Geometry();
	m_geometry->setVertexArray(m_coordinates);

	// seems to be important so that we won't crash after 683 fence parts
	m_geometry->setUseDisplayList(false);
	
	// use the shared normal array.
	// polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);

	m_drawArrays = new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, 0);
	m_geometry->addPrimitiveSet(m_drawArrays);

	m_geode = new osg::Geode();
	m_geode->addDrawable(m_geometry);

	m_node->addChild(m_geode);
}

void FenceView::updateFenceGap(osg::Vec3 lastPosition, osg::Vec3 position)
{
	if (m_coordinates->size() > 1) {
		m_coordinates->at(m_coordinates->size() - 2) = osg::Vec3(position.x(), position.y(), position.z());
		m_coordinates->at(m_coordinates->size() - 1) = osg::Vec3(position.x(), position.y(), position.z() + m_fenceHeight);
	}
}



void FenceView::initializeShader()
{
	osg::ref_ptr<osg::StateSet> NodeState = m_node->getOrCreateStateSet();
	osg::Uniform* fenceColorU = new osg::Uniform("fenceColor", m_playerColor);
	NodeState->addUniform(fenceColorU);

	osg::Uniform* fenceHeightU = new osg::Uniform("fenceHeight", m_fenceHeight);
	NodeState->addUniform(fenceHeightU);

	osg::Uniform* modelIDU = new osg::Uniform("modelID", GLOW);
	NodeState->addUniform(modelIDU);

	NodeState->setMode(GL_BLEND, osg::StateAttribute::ON);
	NodeState->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	NodeState->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::FENCE], osg::StateAttribute::ON);
}

void FenceView::addFencePart(osg::Vec3 lastPosition, osg::Vec3 currentPosition)
{
	if (m_coordinates->size() == 0)
	{
		m_coordinates->push_back(lastPosition);
		m_coordinates->push_back(osg::Vec3(lastPosition.x(), lastPosition.y(), lastPosition.z() + m_fenceHeight));
	}

	m_coordinates->push_back(currentPosition);
	m_coordinates->push_back(osg::Vec3(currentPosition.x(), currentPosition.y(), currentPosition.z() + m_fenceHeight));
	
	enforceFencePartsLimit(m_maxFenceParts);

	// TODO
	// remove if no disadvantages seem necessary?
	// m_geometry->dirtyBound();
	m_drawArrays->setCount(m_coordinates->size());
}

void FenceView::removeAllFences()
{
	m_node->removeChild(m_geode);
	initializeFence();
}

void FenceView::enforceFencePartsLimit(int maxFenceParts)
{
	if (m_maxFenceParts != maxFenceParts)
		m_maxFenceParts = maxFenceParts;

	// the quad strip contains two more vertices for the beginning of the fence
	int currentFenceParts = (m_coordinates->size() - 2) / 2;
	if (maxFenceParts != 0 && currentFenceParts > maxFenceParts)
	{
		for (int i = 0; i < (currentFenceParts - maxFenceParts); i++)
			removeFirstFencePart();
	}
}


void FenceView::removeFirstFencePart()
{
	m_coordinates->erase(m_coordinates->begin(), m_coordinates->begin() + 2);
}
