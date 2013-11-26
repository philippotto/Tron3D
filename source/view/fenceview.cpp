#include "fenceview.h"
// OSG
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Vec4>
// troen
#include "shaders.h"
#include "../model/fencemodel.h"

using namespace troen;

FenceView::FenceView(std::shared_ptr<FenceModel> &model)
{
	m_model = model;
	m_node = new osg::Group();
	initializeFence();
	initializeShader();
}

void FenceView::initializeFence()
{
	m_fenceHeight = m_model->getFenceHeight();

	osg::Geode* geode = new osg::Geode();

	m_geometry = new osg::Geometry();

	// use the shared normal array.
	// polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);

	m_drawArrays = new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, 0);
	m_geometry->addPrimitiveSet(m_drawArrays);

	geode->addDrawable(m_geometry);
	m_node->addChild(geode);
}

void FenceView::initializeShader()
{
	osg::ref_ptr<osg::StateSet> NodeState = m_node->getOrCreateStateSet();

	// TODO (dw) set to actual player color
	osg::Uniform* fenceColorU = new osg::Uniform("fenceColor", osg::Vec3(0.0, 1.0, 1.0));
	NodeState->addUniform(fenceColorU);

	osg::Uniform* fenceHeightU = new osg::Uniform("fenceHeight", m_fenceHeight);
	NodeState->addUniform(fenceHeightU);

	NodeState->setMode(GL_BLEND, osg::StateAttribute::ON);

	NodeState->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::FENCE], osg::StateAttribute::ON);
}

void FenceView::addFencePart(osg::Vec3 a, osg::Vec3 b)
{
	m_coordinates.push_back(b);
	m_coordinates.push_back(osg::Vec3(b.x(), b.y(), b.z() + m_fenceHeight));

	int numCoords = m_coordinates.size();

	osg::Vec3Array* vertices = new osg::Vec3Array(numCoords, m_coordinates.data());

	m_geometry->setVertexArray(vertices);

	// maybe this has to be set as well, but it works without for now
	// m_geometry->dirtyDisplayList();
	m_drawArrays->setCount(numCoords);
}