#include "ragdollview.h"
// OSG
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Texture2D>
#include "osg/MatrixTransform"
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "osg/Math"


// troen
#include "../constants.h"
#include "shaders.h"
#include "levelview.h"
#include "../util/conversionutils.h"
#include "easemotion.h"



using namespace troen;

RagdollView::RagdollView(osg::Vec3 dimensions, osg::Vec3 position)
{
	AbstractView();

	osg::ref_ptr<osg::Box> box
		= new osg::Box(osg::Vec3(0.0, 0.0, 0.0), dimensions.x(), dimensions.y(), dimensions.z());

	osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);

	osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode();
	boxGeode->addDrawable(boxDrawable);

	//osg::StateSet *bodyStateSet = m_node->getOrCreateStateSet();
	//bodyStateSet->ref();
	//osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	//bodyStateSet->addUniform(textureMapU);
	//setTexture(bodyStateSet, "data/textures/white.tga", 0);

	//bodyStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);
	//bodyStateSet->addUniform(new osg::Uniform("levelSize", LEVEL_SIZE));
	//bodyStateSet->addUniform(new osg::Uniform("modelID", AbstractView::GLOW));
	//bodyStateSet->addUniform(new osg::Uniform("trueColor", 1.f));

	osg::Matrixd initialTransform;
	initialTransform = initialTransform.translate(position);

	m_matrixTransform = new osg::MatrixTransform(initialTransform);
	m_matrixTransform->addChild(boxGeode);

	m_node->addChild(m_matrixTransform);


	skelroot = new osgAnimation::Skeleton;
	skelroot->setDefaultUpdateCallback();


	//osgAnimation::Bone* bone0 = createBone("bone0", osg::Vec3(0.0f, 0.0f, 0.0f), skelroot.get());
	//osgAnimation::Bone* bone11 = createBone("bone11", osg::Vec3(0.5f, 0.0f, 0.0f), bone0);
	//osgAnimation::Bone* bone12 = createEndBone("bone12", osg::Vec3(1.0f, 0.0f, 0.0f), bone11);
	//osgAnimation::Bone* bone21 = createBone("bone21", osg::Vec3(-0.5f, 0.0f, 0.0f), bone0);
	//osgAnimation::Bone* bone22 = createEndBone("bone22", osg::Vec3(-1.0f, 0.0f, 0.0f), bone21);
	//osgAnimation::Bone* bone31 = createBone("bone31", osg::Vec3(0.0f, 0.5f, 0.0f), bone0);
	//osgAnimation::Bone* bone32 = createEndBone("bone32", osg::Vec3(0.0f, 1.0f, 0.0f), bone31);
	//osgAnimation::Bone* bone41 = createBone("bone41", osg::Vec3(0.0f, -0.5f, 0.0f), bone0);
	//osgAnimation::Bone* bone42 = createEndBone("bone42", osg::Vec3(0.0f, -1.0f, 0.0f), bone41);


	m_node->addChild(skelroot.get());

}

void RagdollView::setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit)
{

	osg::Image* image = osgDB::readImageFile(filePath);
	if (!image)
		std::cout << "[TroenGame::levelView]  File \"" << filePath << "\" not found." << std::endl;
	else
	{
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);
		texture->setResizeNonPowerOfTwoHint(false);
		stateset->setTextureAttributeAndModes(unit, texture, osg::StateAttribute::ON);
	}
}




osg::ref_ptr<osg::PositionAttitudeTransform>  RagdollView::createBodyPart(btTransform transform, float radius, float height)
{
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = Conversion::transformToPAT(transform);
	
	osg::Cylinder *cylinder = new osg::Cylinder(osg::Vec3(0.0, 0.0, 0.0), radius, height);
	osg::ref_ptr<osg::ShapeDrawable> cylinderDrawable = new osg::ShapeDrawable(cylinder);

	osg::ref_ptr<osg::Geode> cylinderGeode = new osg::Geode();
	cylinderGeode->addDrawable(cylinderDrawable);
	osg::Matrixd turnTransform;

	osg::Quat rotationQuatY(osg::DegreesToRadians(90.0f), osg::Y_AXIS);
	osg::Quat rotationQuatX(osg::DegreesToRadians(90.0f), osg::Z_AXIS);
	turnTransform.makeRotate(rotationQuatY);
	turnTransform *= turnTransform.rotate(rotationQuatX);
	osg::MatrixTransform* matrixTransform = new osg::MatrixTransform(turnTransform);
	
	matrixTransform->addChild(cylinderGeode);
	pat->addChild(matrixTransform);

	m_node->addChild(pat);
	return pat;
}






osg::Geode* RagdollView::createBoneShape(const osg::Vec3& trans, const osg::Vec4& color)
{
	osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	va->push_back(osg::Vec3()); va->push_back(trans);

	osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
	ca->push_back(color);

	osg::ref_ptr<osg::Geometry> line = new osg::Geometry;
	line->setVertexArray(va.get());
	line->setColorArray(ca.get());
	line->setColorBinding(osg::Geometry::BIND_OVERALL);
	line->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 2));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(line.get());
	geode->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(15.0f));
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	return geode.release();
}

osgAnimation::Bone* RagdollView::createBone(const char* name, const osg::Matrix& transform, osg::Group* parent)
{
	osg::ref_ptr<osgAnimation::Bone> bone = new osgAnimation::Bone;
	parent->insertChild(0, bone.get());
	parent->addChild(createBoneShape(transform.getTrans(), osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	osg::ref_ptr<osgAnimation::UpdateBone> updater = new osgAnimation::UpdateBone(name);
	updater->getStackedTransforms().push_back(new osgAnimation::StackedTranslateElement("translate", transform.getTrans()));
	updater->getStackedTransforms().push_back(new osgAnimation::StackedQuaternionElement("quaternion",transform.getRotate()));

	bone->setUpdateCallback(updater.get());
	bone->setMatrixInSkeletonSpace(osg::Matrix::translate(transform.getTrans()) * bone->getMatrixInSkeletonSpace());
	bone->setName(name);
	return bone.get();
}

osgAnimation::Bone* RagdollView::createEndBone(const char* name, const osg::Matrix& transform, osg::Group* parent)
{
	osgAnimation::Bone* bone = createBone(name, transform, parent);
	bone->addChild(createBoneShape(transform.getTrans(), osg::Vec4(0.4f, 1.0f, 0.4f, 1.0f)));
	return bone;
}

osgAnimation::Channel* RagdollView::createChannel(const char* name, const osg::Vec3& axis, float rad)
{
	osg::ref_ptr<osgAnimation::QuatSphericalLinearChannel> ch = new osgAnimation::QuatSphericalLinearChannel;
	ch->setName("quaternion");
	ch->setTargetName(name);

	osgAnimation::QuatKeyframeContainer* kfs = ch->getOrCreateSampler()->getOrCreateKeyframeContainer();
	kfs->push_back(osgAnimation::QuatKeyframe(0.0, osg::Quat(0.0, axis)));
	kfs->push_back(osgAnimation::QuatKeyframe(8.0, osg::Quat(rad, axis)));
	return ch.release();
}
