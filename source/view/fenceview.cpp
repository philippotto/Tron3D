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
	m_fenceHeight = m_model.lock()->getFenceHeight();

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

	// fenceGap
	
	initializeFenceGap();
	m_node->addChild(m_geode);
	m_node->addChild(m_gapPat);
	m_node->setCullingActive(false);
}

void FenceView::initializeFenceGap() {
	
	if (m_gapPat)
		// don't reinitialize fence gap
		return;

	m_gapPat = new osg::PositionAttitudeTransform();
	m_gapPat->setPosition(osg::Vec3(2.5, 0, 0));

	osg::ref_ptr<osg::Geometry> quadGeometry = osg::createTexturedQuadGeometry(osg::Vec3(0.0f, -0.5f, 0.0f),
		osg::Vec3(0.f, 1.f, 0.0f),
		osg::Vec3(0.0f, 0.0f, m_fenceHeight),
		0.0f, 0.0f, 1.0f, 1.0f);

	m_fenceGap = new osg::Geode();
	m_fenceGap->addDrawable(quadGeometry);
	m_gapPat->addChild(m_fenceGap);
}

void FenceView::updateFenceGap(osg::Vec3 lastPosition, osg::Vec3 position) {
	
	m_gapPat->setPosition((position + lastPosition) / 2);
	m_gapPat->setScale(osg::Vec3(1.f, (position - lastPosition).length(), 1.f));
	
	if (lastPosition != position) {
		osg::Quat rotationQuat;
		osg::Vec3 fenceVector = position - lastPosition;
		const osg::Vec3 forward = osg::Vec3(0, 1, 0);
		rotationQuat.makeRotate(forward, fenceVector);
		m_gapPat->setAttitude(rotationQuat);
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
	NodeState->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::FENCE], osg::StateAttribute::ON);
}

void FenceView::addFencePart(osg::Vec3 lastPosition, osg::Vec3 currentPosition)
{
	if (m_coordinates->size()==0)
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
	m_node->removeChild(m_gapPat);
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
