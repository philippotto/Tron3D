#pragma once

#include <osg/MatrixTransform>
#include <osg/TextureCubeMap>

namespace osg {
    class TexMat;
    class TextureCubeMap;
}

namespace troen {

	/**
		* @brief Representation of Skybox
		*
		* Visualizes the sky in the game, through projecting a cubemap onto a Sphere.
		*/
	class SkyDome : public osg::MatrixTransform {
	public:
		SkyDome();
		SkyDome(const SkyDome& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
		META_Node(a3d, SkyDome);
		osg::ref_ptr<osg::TextureCubeMap> readCubeMap();
		osg::ref_ptr<osg::TextureCubeMap> getSkyboxTexture();
	protected:
		virtual ~SkyDome();
		osg::ref_ptr<osg::TextureCubeMap> m_texture;
		

	};

	/**
		* @brief Update texture matrix for cubemaps
		*/
	class SkyboxTexMatCallback : public osg::NodeCallback {

	public:
		SkyboxTexMatCallback();
		SkyboxTexMatCallback(const SkyboxTexMatCallback& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
		META_Object(a3d, SkyboxTexMatCallback);
		virtual void operator()(osg::Node *node, osg::NodeVisitor* nv);

	protected:
		virtual ~SkyboxTexMatCallback() {}
	};

}