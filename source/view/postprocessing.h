#pragma once
// OSG
#include <osg/Group>
#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <osg/Shader>
#include <osg/Program>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"

#include "../oculus/oculusdevice.h"
#include <OVR.h>

namespace troen
{

	class PostProcessing : public AbstractView
	{
	public:
		PostProcessing(osg::ref_ptr<osg::Group> rootNode, const int width, const int height, OculusDevice *od);

		enum TEXTURE_CONTENT { COLOR, ID, PING, PONG, OLDCOLOR, LEFT, RIGHT, TEXTURE_CONTENT_SIZE };

		osg::ref_ptr<osg::Group> getSceneNode() { return m_sceneNode; };
		osg::ref_ptr<osg::Camera> pingPongPass(const int order, const TEXTURE_CONTENT inputTexture, const TEXTURE_CONTENT outputTexture, const int type, const int step);
		void setupTextures(const unsigned int & width, const unsigned int &height);
		bool handleGuiEvents(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*);

		void attachGBufferShaderProgram(osg::ref_ptr<osg::StateSet> state);
		void attachPostShaderProgram(osg::ref_ptr<osg::StateSet> state);
		//void attachSkeletonShaderProgram(osg::ref_ptr<osg::StateSet> state);
		//void attachDistanceTransformShaderProgram(osg::ref_ptr<osg::StateSet> state, TEXTURE_CONTENT inputTexture, TEXTURE_CONTENT outputTexture, SHADER_PROGRAM_TYPES type, int step);

		osg::Uniform* m_timeSinceLastBeat;
		void setBeat(const float beat);

		// Oculus specific Code
		bool m_useOculus = true;
		OculusDevice* m_device;
		/*OVR::HMDInfo* m_hmd;*/

	protected:
		osg::ref_ptr<osg::Camera> gBufferPass();
		osg::ref_ptr<osg::Camera> oculusPass(bool left);
		osg::ref_ptr<osg::Camera> PostProcessing::mergeEyes();
		osg::ref_ptr<osg::Camera> postProcessingPass();
		//osg::ref_ptr<osg::Camera> distanceTransformPass(int order, TEXTURE_CONTENT inputTexture, TEXTURE_CONTENT outputTexture, SHADER_PROGRAM_TYPES type, int step);

		std::vector<osg::ref_ptr<osg::Texture2D> > m_fboTextures;
		std::vector<osg::ref_ptr<osg::Camera> > m_allCameras;

		osg::ref_ptr<osg::Group> m_root;
		osg::ref_ptr<osg::Group> m_sceneNode;

		int m_width;
		int m_height;
		std::vector<int> pingpongPasses;
	};
}