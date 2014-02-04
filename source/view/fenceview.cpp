#include "fenceview.h"
// OSG
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Vec4>
#include <osg/PositionAttitudeTransform>
#include <osg/ref_ptr>

// troen
#include "../constants.h"
#include "shaders.h"
#include "../model/fencemodel.h"
#include "../controller/fencecontroller.h"

using namespace troen;

FenceView::FenceView(FenceController* fenceController, osg::Vec3 color, std::shared_ptr<AbstractModel>& model)
{
	AbstractView();
	m_playerColor = color;
	m_model = std::static_pointer_cast<FenceModel>(model);
	m_node = new osg::Group();

	m_fenceController = fenceController;

	initializeFence();
	initializeShader();
}

void FenceView::initializeFence()
{
	m_fenceHeight = FENCE_HEIGHT_VIEW;

	m_coordinates = new osg::Vec3Array;
	m_coordinates->setDataVariance(osg::Object::DYNAMIC);

	m_relativeHeights = new osg::FloatArray;
	m_relativeHeights->setDataVariance(osg::Object::DYNAMIC);

	m_geometry = new osg::Geometry();
	m_geometry->setVertexArray(m_coordinates);

	// set the relative height between 0 and 1 as an additional vertex attribute
	m_geometry->setVertexAttribArray(5, m_relativeHeights);
	m_geometry->setVertexAttribBinding(5, osg::Geometry::BIND_PER_VERTEX);

	// seems to be important so that we won't crash after 683 fence parts
	m_geometry->setUseDisplayList(false);
	
	// use the shared normal array.
	// polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);

	m_drawArrays = new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, 0);
	m_geometry->addPrimitiveSet(m_drawArrays);

	m_geode = new osg::Geode();
	m_geode->addDrawable(m_geometry);

	m_node->addChild(m_geode);
	m_node->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
	m_node->setName("fenceGroup");
	
}

void FenceView::updateFenceGap(osg::Vec3 lastPosition, osg::Vec3 position)
{
	if (m_coordinates->size() > 1) {
		m_coordinates->at(m_coordinates->size() - 2) = osg::Vec3(position.x(), position.y(), position.z());
		m_coordinates->at(m_coordinates->size() - 1) = osg::Vec3(position.x(), position.y(), position.z() + m_fenceHeight);
		m_relativeHeights->at(m_relativeHeights->size() - 2) = 0.f;
		m_relativeHeights->at(m_relativeHeights->size() - 1) = 1.f;
	}
}

void FenceView::initializeShader()
{
	osg::ref_ptr<osg::StateSet> NodeState = m_node->getOrCreateStateSet();
	osg::Uniform* fenceColorU = new osg::Uniform("fenceColor", m_playerColor);
	NodeState->addUniform(fenceColorU);

	osg::Uniform* modelIDU = new osg::Uniform("modelID", GLOW);
	NodeState->addUniform(modelIDU);

	NodeState->setMode(GL_BLEND, osg::StateAttribute::ON);
	NodeState->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	NodeState->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::FENCE], osg::StateAttribute::ON);

	shaders::m_allShaderPrograms[shaders::FENCE]->addBindAttribLocation("a_relHeight", 5);
}

void FenceView::addFencePart(osg::Vec3 lastPosition, osg::Vec3 currentPosition)
{
	if (m_coordinates->size() == 0)
	{
		m_coordinates->push_back(lastPosition);
		m_coordinates->push_back(osg::Vec3(lastPosition.x(), lastPosition.y(), lastPosition.z() + m_fenceHeight));
		m_relativeHeights->push_back(0.f);
		m_relativeHeights->push_back(1.f);
	}

	m_coordinates->push_back(currentPosition);
	m_coordinates->push_back(osg::Vec3(currentPosition.x(), currentPosition.y(), currentPosition.z() + m_fenceHeight));
	m_relativeHeights->push_back(0.f);
	m_relativeHeights->push_back(1.f);
	
	enforceFencePartsLimit();

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

void FenceView::enforceFencePartsLimit()
{
	int maxFenceParts = m_fenceController->getFenceLimit();

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
	m_relativeHeights->erase(m_relativeHeights->begin(), m_relativeHeights->begin() + 2);
}
