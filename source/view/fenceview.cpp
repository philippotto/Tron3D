#include "fenceview.h"
// OSG
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Vec4>

using namespace troen;

FenceView::FenceView()
{
	m_node = new osg::Group();
	initializeFence();
}

void FenceView::initializeFence()
{
	osg::Geode* geode = new osg::Geode();

	m_geometry = new osg::Geometry();

	// use the shared normal array.
	// polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);

	m_drawArrays = new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, 0);
	m_geometry->addPrimitiveSet(m_drawArrays);

	geode->addDrawable(m_geometry);
	m_node->addChild(geode);
}

void FenceView::addFencePart(osg::Vec3 a, osg::Vec3 b)
{
	m_coordinates.push_back(b);
	m_coordinates.push_back(osg::Vec3(b.x(), b.y(), b.z() + 20.0));

	int numCoords = m_coordinates.size();

	osg::Vec3Array* vertices = new osg::Vec3Array(numCoords, m_coordinates.data());

	m_geometry->setVertexArray(vertices);

	// maybe this has to be set as well, but it works without for now
	// m_geometry->dirtyDisplayList();
	m_drawArrays->setCount(numCoords);
}