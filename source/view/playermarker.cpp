#include "playermarker.h"
// OSG
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Vec4>
#include <osg/PositionAttitudeTransform>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TextureCubeMap>
#include <osg/TexMat>
#include <osg/Geode>
#include <osgDB/WriteFile>
#include <osg/CullFace>
#include <osg/TexGenNode>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/BlendFunc>

#include <osgUtil/CullVisitor>
// troen
#include "../constants.h"
#include "shaders.h"


using namespace troen;

PlayerMarker::PlayerMarker(osg::Vec3 color)
{
	AbstractView();
	m_playerColor = color;
	m_node = new osg::Group();
	osg::Matrixd initialTransform;
	initialTransform.makeScale(osg::Vec3(3.0, 3.0, 3.0));
	initialTransform *= initialTransform.translate(osg::Vec3(5.0, 0.0, 30.0));
	osg::MatrixTransform* matrixTransform = new osg::MatrixTransform(initialTransform);
	

	osg::Group* arrowNode = dynamic_cast<osg::Group*>(osgDB::readNodeFile("data/models/arrow.obj"));

	std::cout << "[TroenGame::playerMarker] Loading Arrow " << std::endl;
	matrixTransform->addChild(arrowNode);
	m_node->addChild(matrixTransform);

	osg::ref_ptr<osg::StateSet> NodeState = arrowNode->getOrCreateStateSet();

	osg::ref_ptr<osg::Geode> singleGeode = dynamic_cast<osg::Geode*>(arrowNode->asGroup()->getChild(0));
	osg::ref_ptr<osg::StateSet> childState = singleGeode->getDrawable(0)->getStateSet();
	osg::StateAttribute* stateAttributeMaterial = childState->getAttribute(osg::StateAttribute::MATERIAL);

	if (stateAttributeMaterial != nullptr)
	{
		osg::Material *objMaterial = dynamic_cast<osg::Material*>(stateAttributeMaterial);
		objMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(color, 1.0));
	


		childState->setAttributeAndModes(new osg::BlendFunc);
		childState->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		arrowNode->addCullCallback(new FadeInOutCallback(objMaterial));
	
	}
}


osg::ref_ptr<osg::Group> PlayerMarker::getNode()
{
	return m_node;
}

FadeInOutCallback::FadeInOutCallback( osg::Material* mat )
: _material(mat), _lastDistance(-1.0f), _fadingState(0)
{
	_motion = new osgAnimation::InOutCubicMotion;
}

void FadeInOutCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	if (_fadingState!=0)
	{
		_motion->update(0.01);
		float value = _motion->getValue();
		float alpha = (_fadingState>0 ? value : 1.0f - value);
		_material->setAlpha( osg::Material::FRONT_AND_BACK,alpha);

		if (value>= 1.0)
			_fadingState = 0;
		traverse(node,nv);
		return;
	}
	osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>( nv );
	if ( cv )
	{
		float distance = cv->getDistanceFromEyePoint(
		node->getBound().center(), true );
		if ( _lastDistance>0.0f )
		{
			if ( _lastDistance<200.0f && distance>=200.0f )
			{
				_fadingState = 1; _motion->reset();
			}
			else if ( _lastDistance>200.0f && distance<=200.0f )
			{
				_fadingState =-1; _motion->reset();
			}
			else if (_lastDistance < 50 && distance < 50)
			{
				_material->setAlpha(osg::Material::FRONT_AND_BACK, 0.0);
			}
		}
		_lastDistance = distance;
	}
	traverse( node, nv );
}