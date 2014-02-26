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

FenceView::FenceView(FenceController* fenceController, osg::Vec3 color, std::shared_ptr<AbstractModel>& model) :
AbstractView(),
m_model(std::static_pointer_cast<FenceModel>(model)),
m_fenceController(fenceController),
m_playerColor(color)
{
	initializeFence();
	initializeShader();
}

void FenceView::initializeFence()
{
	m_fenceHeight = FENCE_HEIGHT_VIEW;

	m_coordinates = new osg::Vec3Array();
	m_coordinates->setDataVariance(osg::Object::DYNAMIC);

	m_relativeHeights = new osg::FloatArray();
	m_relativeHeights->setDataVariance(osg::Object::DYNAMIC);

	// this value could need adaption; will avoid time-intensive array resizing
	m_coordinates->reserveArray(10000);
	m_relativeHeights->reserveArray(10000);

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

	m_radarElementsGroup = new osg::Group();
	m_radarElementsGroup->setNodeMask(CAMERA_MASK_NONE);
	m_node->addChild(m_radarElementsGroup);
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

	// game fence part
	m_coordinates->push_back(currentPosition);
	m_coordinates->push_back(osg::Vec3(currentPosition.x(), currentPosition.y(), currentPosition.z() + m_fenceHeight));
	m_relativeHeights->push_back(0.f);
	m_relativeHeights->push_back(1.f);
	
	// radar fence part
	osg::ref_ptr<osg::Box> box
		= new osg::Box(osg::Vec3(0, 0, 0), 50, 50, 50);
	osg::ref_ptr<osg::ShapeDrawable> mark_shape = new osg::ShapeDrawable(box);
	mark_shape->setColor(osg::Vec4f(m_playerColor, 1));
	osg::ref_ptr<osg::Geode> mark_node = new osg::Geode;
	mark_node->addDrawable(mark_shape.get());
	//mark_node->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	// place objects in world space
	osg::Matrixd initialTransform;
	//initialTransform.makeRotate(rotationQuatXY);
	initialTransform *= initialTransform.translate((currentPosition + lastPosition) / 2);

	osg::ref_ptr<osg::MatrixTransform> matrixTransformRadar = new osg::MatrixTransform(initialTransform);
	matrixTransformRadar->addChild(mark_node);

	m_radarElementsGroup->addChild(matrixTransformRadar);
	m_radarFenceBoxes.push_back(matrixTransformRadar);

	// limit
	enforceFencePartsLimit();

	// TODO
	// remove if no disadvantages seem necessary?
	// m_geometry->dirtyBound();
	m_drawArrays->setCount(m_coordinates->size());
}

void FenceView::removeAllFences()
{
	m_node->removeChild(m_geode);
	for (auto radarFenceBox : m_radarFenceBoxes)
	{
		m_radarElementsGroup->removeChild(radarFenceBox);
	}
	m_radarFenceBoxes.clear();
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
		{
			m_coordinates->erase(m_coordinates->begin(), m_coordinates->begin() + 2);
			m_relativeHeights->erase(m_relativeHeights->begin(), m_relativeHeights->begin() + 2);
		}
	}
	// radar fence boxes
	if (maxFenceParts != 0 && m_radarFenceBoxes.size() > maxFenceParts)
	{
		for (int i = 0; i < (m_radarFenceBoxes.size() - maxFenceParts); i++)
		{
			m_radarElementsGroup->removeChild(m_radarFenceBoxes.front());
			m_radarFenceBoxes.pop_front();		
		}
	}
}

void FenceView::showFencesInRadarForPlayer(const int id)
{
	osg::Node::NodeMask currentMask = m_radarElementsGroup->getNodeMask();
	osg::Node::NodeMask newMask = currentMask | CAMERA_MASK_PLAYER[id];
	m_radarElementsGroup->setNodeMask(newMask);
}

void FenceView::hideFencesInRadarForPlayer(const int id)
{	
	osg::Node::NodeMask currentMask = m_radarElementsGroup->getNodeMask();
	osg::Node::NodeMask newMask = currentMask & ~ CAMERA_MASK_PLAYER[id];
	m_radarElementsGroup->setNodeMask(newMask);
}