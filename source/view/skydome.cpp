#include "SkyDome.h"
//#include "constants.h"

#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TexMat>
#include <osg/TextureCubeMap>
#include <osg/Depth>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osgDB/ReadFile>
#include <osgUtil/CullVisitor>

using namespace troen;

SkyDome::SkyDome() :
        osg::MatrixTransform()
{
	int sphereSize = 50000;
	osg::Vec3 sphereOffset = osg::Vec3(0.0f, 0.0f, -5000.0f);
	osg::ref_ptr<osg::Drawable> drawable = new osg::ShapeDrawable(new osg::Sphere(sphereOffset, sphereSize));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->setCullingActive(false);
    geode->setCullCallback(new SkyboxTexMatCallback);
    geode->addDrawable(drawable.get());
    //geode->setNodeMask(CameraMask::kMain);

    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateset->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
    stateset->setTextureAttributeAndModes(0, readCubeMap(), osg::StateAttribute::ON);
    stateset->setTextureAttributeAndModes(0, new osg::TexEnv(osg::TexEnv::REPLACE), osg::StateAttribute::ON);
    stateset->setTextureAttributeAndModes(0, new osg::TexMat, osg::StateAttribute::ON);

    osg::ref_ptr<osg::TexGen> texGen = new osg::TexGen;
    texGen->setMode(osg::TexGen::NORMAL_MAP);
    stateset->setTextureAttributeAndModes(0, texGen.get(), osg::StateAttribute::ON);

    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader(osgDB::readShaderFile("source/shaders/skydome.frag"));
    //program->addShader(osgDB::readShaderFile("shader/TypeId.frag"));
    stateset->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
    stateset->addUniform(new osg::Uniform("cubemap", 0));

    // clear the depth to the far plane.
    osg::ref_ptr<osg::Depth> depth = new osg::Depth(osg::Depth::ALWAYS, 1.0, 1.0);
    stateset->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);

    osg::ref_ptr<osg::ClearNode> clearNode = new osg::ClearNode();
    clearNode->setRequiresClear(false);
    clearNode->addChild(geode.get());

    addChild(clearNode.get());
}

SkyDome::SkyDome(const SkyDome& copy, const osg::CopyOp& copyop) :
        osg::MatrixTransform(copy, copyop) {
}

SkyDome::~SkyDome() {
}

osg::TextureCubeMap* SkyDome::readCubeMap() {
	#define SKYBOX_FILENAME(face) "data/textures/skybox/" #face ".bmp"

    osg::ref_ptr<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap();
	cubemap->setResizeNonPowerOfTwoHint(false);

	osg::ref_ptr<osg::Image> imagePosX = osgDB::readImageFile(SKYBOX_FILENAME(px));
	osg::ref_ptr<osg::Image> imageNegX = osgDB::readImageFile(SKYBOX_FILENAME(nx));
	osg::ref_ptr<osg::Image> imagePosY = osgDB::readImageFile(SKYBOX_FILENAME(py));
	osg::ref_ptr<osg::Image> imageNegY = osgDB::readImageFile(SKYBOX_FILENAME(ny));
	osg::ref_ptr<osg::Image> imagePosZ = osgDB::readImageFile(SKYBOX_FILENAME(pz));
	osg::ref_ptr<osg::Image> imageNegZ = osgDB::readImageFile(SKYBOX_FILENAME(nz));

	// Sanity Check
    if (imagePosX && imageNegX && imagePosY && imageNegY && imagePosZ && imageNegZ) {
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX);
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY);
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ);

        cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

        cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
        cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

		//OSG_NOTICE << "Skybox: Loaded correctly.\n";
    } else OSG_NOTICE << "Skybox: Error while loading image files!\n";

    return cubemap.release();
}

SkyboxTexMatCallback::SkyboxTexMatCallback() :
        osg::NodeCallback() {
}

SkyboxTexMatCallback::SkyboxTexMatCallback(const SkyboxTexMatCallback& copy, const osg::CopyOp& copyop) :
        osg::NodeCallback(copy, copyop) {
}

void SkyboxTexMatCallback::operator()(osg::Node *node, osg::NodeVisitor* nv) {
    osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
    if (cv != NULL) {
        osg::TexMat* texMat = NULL;
        osg::StateSet* stateSet = node->getStateSet();
        if (stateSet != NULL) {
            texMat = static_cast<osg::TexMat*>(stateSet->getTextureAttribute(0, osg::StateAttribute::TEXMAT));
        }
        if (texMat != NULL) {
            osg::RefMatrix* modelViewMatrix = cv->getModelViewMatrix();
            osg::Quat rotation = modelViewMatrix->getRotate();
            osg::Matrix inverseRotationMatrix = osg::Matrix::rotate(rotation.inverse());

            texMat->setMatrix(inverseRotationMatrix);
        }
    }

    traverse(node,nv);
}
