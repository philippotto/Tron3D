#include "postprocessing.h"
//osg
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TextureCubeMap>
#include <osg/TexMat>
#include <osg/Material>
#include <osg/Geode>
#include <osgDB/WriteFile>
#include <osg/CullFace>
#include <osg/TexGenNode>
#include <osgGA/StateSetManipulator>
#include <osgDB/FileNameUtils>
#include <stdio.h>

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
// troen

#include "shaders.h"
// ugly but convenient global statics for shaders    
static osg::ref_ptr<osg::Uniform> g_nearFarUniform = new osg::Uniform("nearFar", osg::Vec2(0.0, 1.0));
#include "nearfarcallback.h"
#include "timeupdate.h"

#define HALF_PINGPONGTEXTURE_WIDTH true //for performance improvement, set to true

using namespace troen;

PostProcessing::PostProcessing(osg::ref_ptr<osg::Group> rootNode, int width, int height, OVR::HMDInfo* hmd, osg::View* view)
:m_root(rootNode), m_sceneNode(new osg::Group()), m_width(width), m_height(height), m_hmd(hmd), m_view(view)
{
	AbstractView();
	// init textures, will be recreated when screen size changes
	setupTextures(m_width, m_height);

	// create shaders
	shaders::reloadShaders();

	
	if (m_useOculus) {
		unsigned int pass = 0;
		

		// TODO: deactivate this and implement proper culling!
		m_root->setCullingActive(false);
		m_sceneNode->setCullingActive(false);

		m_allCameras.push_back(oculusPass(true));
		m_root->addChild(m_allCameras[pass++]);
		
		m_allCameras.push_back(oculusPass(false));
		m_root->addChild(m_allCameras[pass++]);

		

		m_allCameras.push_back(mergeEyes());
		m_root->addChild(m_allCameras[pass++]);
	}
	else {


		////////////////////////////////////////
		// Multi pass rendering and Ping Pong //
		////////////////////////////////////////

		// 1. gBuffer pass: render color, normal & depth, id buffer
		unsigned int pass = 0;
		m_allCameras.push_back(gBufferPass());
		m_root->addChild(m_allCameras[pass++]);

		// 2. prepare pass: render id buffer as seeds into PONG texture
		//TEXTURE_CONTENT pingPong[] = { PING, PONG };
		// start writing into PONG buffer (pass == 1 )

		m_allCameras.push_back(pingPongPass(pass, COLOR, PONG, shaders::SELECT_GLOW_OBJECTS, -1.0));
		m_root->addChild(m_allCameras[pass++]);

		m_allCameras.push_back(pingPongPass(pass, PONG, PING, shaders::HBLUR, -1.0));
		m_root->addChild(m_allCameras[pass++]);

		m_allCameras.push_back(pingPongPass(pass, PING, PONG, shaders::VBLUR, -1.0));
		m_root->addChild(m_allCameras[pass++]);

		m_allCameras.push_back(postProcessingPass());
		m_root->addChild(m_allCameras[m_allCameras.size() - 1]);
	}
}

// sets up textures
void PostProcessing::setupTextures(const unsigned int & width, const unsigned int &height)
{
	//////////////////////////////////////////////////////////////////////////
	// 2D textures as render targets
	//////////////////////////////////////////////////////////////////////////

	int halfedWidth = width / 2;
	int halfedHeight = height / 2;

	// store color, normal & Depth, id in textures
	m_fboTextures.resize(TEXTURE_CONTENT_SIZE);
	for (int i = 0; i<m_fboTextures.size(); i++)
	{
		// only create textures on first run

		if (!m_fboTextures[i].get()) {
			m_fboTextures[i] = new osg::Texture2D();
		}
		
		if (m_useOculus) {

			if (i == LEFT || i == RIGHT)
				m_fboTextures[i]->setTextureWidth(halfedWidth);
			else
				m_fboTextures[i]->setTextureWidth(width);
			
			m_fboTextures[i]->setTextureHeight(height);

			m_fboTextures[i]->setInternalFormat(GL_RGBA);
			m_fboTextures[i]->setSourceFormat(GL_RGBA);
			m_fboTextures[i]->setSourceType(GL_FLOAT);

		} else {


			if ((i == PING || i == PONG) && HALF_PINGPONGTEXTURE_WIDTH) {
				m_fboTextures[i]->setTextureWidth(halfedWidth);
				m_fboTextures[i]->setTextureHeight(halfedHeight);
			}
			else {
				m_fboTextures[i]->setTextureWidth(width);
				m_fboTextures[i]->setTextureHeight(height);
			}
		
			// higher resolution
			if (i == ID)
			{
				m_fboTextures[i]->setInternalFormat(GL_RG);
				m_fboTextures[i]->setSourceFormat(GL_RG);
				m_fboTextures[i]->setSourceType(GL_FLOAT);
			}
			else
			{
				m_fboTextures[i]->setInternalFormat(GL_RGBA);
				m_fboTextures[i]->setSourceFormat(GL_RGBA);
				m_fboTextures[i]->setSourceType(GL_FLOAT);
			}
		}
		m_fboTextures[i]->setBorderWidth(0);
		m_fboTextures[i]->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
		m_fboTextures[i]->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
		m_fboTextures[i]->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
		m_fboTextures[i]->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
		m_fboTextures[i]->setBorderColor(osg::Vec4d(0.0, 0.0, 0.0, 1.0));
		m_fboTextures[i]->setResizeNonPowerOfTwoHint(true);

		// important to reflect the change in size
		m_fboTextures[i]->dirtyTextureObject();
	}
	
	
	
	// important to reflect the change in size for the FBO
	if (m_allCameras.size() > 0)
	{
		if (m_useOculus) {
			m_allCameras[0]->setRenderingCache(0);
			m_allCameras[1]->setRenderingCache(0);
			m_allCameras[2]->setRenderingCache(0);


			// TODO: remove this line
			//halfedWidth = width;

			m_allCameras[0]->setViewport(new osg::Viewport(0, 0, halfedWidth, height));
			m_allCameras[1]->setViewport(new osg::Viewport(0, 0, halfedWidth, height));


			/*m_allCameras[0]->setViewport(new osg::Viewport(0, 0, halfedWidth, height));
			m_allCameras[1]->setViewport(new osg::Viewport(halfedWidth, 0, halfedWidth, height));*/
			m_allCameras[2]->setViewport(new osg::Viewport(0, 0, width, height));

			
		}
		else {
			
			for (size_t i = 0, iEnd = m_allCameras.size(); i < iEnd; i++)
			{
				m_allCameras[i]->setRenderingCache(0);
				if (i  != 0 && i != iEnd - 1 && HALF_PINGPONGTEXTURE_WIDTH)
					// only draw with halfed resolution, if we process the gbuffer + postprocessing pass
					m_allCameras[i]->setViewport(new osg::Viewport(0, 0, halfedWidth, halfedHeight));
			}
		}
	}
}


// create gbuffer creation camera
osg::ref_ptr<osg::Camera> PostProcessing::gBufferPass()
{

	osg::ref_ptr<osg::Camera> cam = new osg::Camera();
	// output textures
	cam->attach((osg::Camera::BufferComponent)(osg::Camera::COLOR_BUFFER0 + COLOR), m_fboTextures[COLOR]);
	//cam->attach((osg::Camera::BufferComponent)(osg::Camera::COLOR_BUFFER0 + NORMALDEPTH), m_fboTextures[NORMALDEPTH]);
	cam->attach((osg::Camera::BufferComponent)(osg::Camera::COLOR_BUFFER0 + ID), m_fboTextures[ID]);

	// Configure fboCamera to draw fullscreen textured quad
	// black clear color
	cam->setClearColor(osg::Vec4(0.0, 0.0, 0.5, 1.0));
	cam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

	cam->setReferenceFrame(osg::Camera::RELATIVE_RF);
	cam->setRenderOrder(osg::Camera::POST_RENDER, 0);

	// need to know about near far changes for correct depth 
	//cam->setCullCallback(new NearFarCallback());
	cam->addChild(m_sceneNode);

	// attach shader program
	//cam->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	//cam->getOrCreateStateSet()->addUniform(new osg::Uniform("colorTex", COLOR));
	//cam->getOrCreateStateSet()->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::GBUFFER], osg::StateAttribute::ON);
	//cam->getOrCreateStateSet()->setTextureAttributeAndModes(COLOR, m_fboTextures[COLOR], osg::StateAttribute::ON);
	//cam->getOrCreateStateSet()->setTextureAttributeAndModes(NORMALDEPTH, m_fboTextures[NORMALDEPTH], osg::StateAttribute::ON);
	//cam->getOrCreateStateSet()->setTextureAttributeAndModes(ID, m_fboTextures[ID], osg::StateAttribute::ON);

	//g_nearFarUniform = new osg::Uniform("nearFar", osg::Vec2(0.0, 1.0));
	//cam->getOrCreateStateSet()->addUniform(g_nearFarUniform);

	return cam;
}

// create skeleton creation camera 
osg::ref_ptr<osg::Camera> PostProcessing::pingPongPass(int order, TEXTURE_CONTENT inputTexture, TEXTURE_CONTENT outputTexture, int shader, int step)
{
	osg::ref_ptr<osg::Camera> camera(new osg::Camera());

	// output textures	
	camera->attach((osg::Camera::BufferComponent) (osg::Camera::COLOR_BUFFER0), m_fboTextures[outputTexture]);

	// Configure fboCamera to draw fullscreen textured quad
	camera->setClearColor(osg::Vec4(0.0, float(float(order) / float(m_allCameras.size())), 0.0, 1.0));
	camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

	camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	camera->setRenderOrder(osg::Camera::POST_RENDER, order);

	// geometry
	osg::Geode* geode(new osg::Geode());
	geode->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(-1, -1, 0), osg::Vec3(2, 0, 0), osg::Vec3(0, 2, 0)));
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	camera->addChild(geode);

	// attach shader program
	osg::ref_ptr<osg::StateSet>	state = camera->getOrCreateStateSet();
	state->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	state->setAttributeAndModes(shaders::m_allShaderPrograms[shader], osg::StateAttribute::ON);

	// add sampler textures	
	state->addUniform(new osg::Uniform("inputLayer", inputTexture));
	state->addUniform(new osg::Uniform("idLayer", ID));
	if (step != -1) state->addUniform(new osg::Uniform("currentStep", step));


	// add time uniform
	osg::Uniform* timeU = new osg::Uniform("time", 0.f);
	state->addUniform(timeU);
	timeU->setUpdateCallback(new TimeUpdate());

	// add beat uniform
	m_timeSinceLastBeat = new osg::Uniform("timeSinceLastBeat", 0.5f);
	state->addUniform(m_timeSinceLastBeat);

	state->setTextureAttributeAndModes(inputTexture, m_fboTextures[inputTexture], osg::StateAttribute::ON);
	state->setTextureAttributeAndModes(ID, m_fboTextures[ID], osg::StateAttribute::ON);

	return camera.release();
}

void PostProcessing::setBeat(float beat)
{
	//m_timeSinceLastBeat->set(beat);
}

// create post processing pass to put it all together
osg::ref_ptr<osg::Camera> PostProcessing::postProcessingPass()
{
	osg::ref_ptr<osg::Camera> postRenderCamera(new osg::Camera());

	// input textures
	postRenderCamera->attach((osg::Camera::BufferComponent) (osg::Camera::COLOR_BUFFER0 + OLDCOLOR), m_fboTextures[OLDCOLOR]);
	//postRenderCamera->attach((osg::Camera::BufferComponent) (osg::Camera::COLOR_BUFFER0 + NORMALDEPTH), m_fboTextures[NORMALDEPTH]);
	//postRenderCamera->attach((osg::Camera::BufferComponent) (osg::Camera::COLOR_BUFFER0 + ID), m_fboTextures[ID]);
	//postRenderCamera->attach((osg::Camera::BufferComponent) (osg::Camera::COLOR_BUFFER0 + PONG), m_fboTextures[PONG]);

	// configure postRenderCamera to draw fullscreen textured quad
	postRenderCamera->setClearColor(osg::Vec4(0.0, 0.5, 0.0, 1)); // should never see this.
	postRenderCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	postRenderCamera->setRenderOrder(osg::Camera::POST_RENDER);

	// geometry
	osg::Geode* geode(new osg::Geode());
	geode->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(-1, -1, 0), osg::Vec3(2, 0, 0), osg::Vec3(0, 2, 0)));
	//geode->getOrCreateStateSet()->setTextureAttributeAndModes(COLOR, m_fboTextures[COLOR], osg::StateAttribute::ON);
	//geode->getOrCreateStateSet()->setTextureAttributeAndModes(NORMALDEPTH, m_fboTextures[NORMALDEPTH], osg::StateAttribute::ON);
	/*geode->getOrCreateStateSet()->setTextureAttributeAndModes(ID, m_fboTextures[ID], osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(PONG, m_fboTextures[PONG], osg::StateAttribute::ON);*/
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	postRenderCamera->addChild(geode);

	// attach shader program
	osg::ref_ptr<osg::StateSet>	state = postRenderCamera->getOrCreateStateSet();
	state->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	state->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::POST_PROCESSING], osg::StateAttribute::ON);

	// add samplers	
	state->addUniform(new osg::Uniform("sceneLayer", COLOR));
	//state->addUniform(new osg::Uniform("normalDepthLayer", NORMALDEPTH));
	state->addUniform(new osg::Uniform("idLayer", ID));
	state->addUniform(new osg::Uniform("pongLayer", PONG));
	state->addUniform(new osg::Uniform("oldLayer", OLDCOLOR));

	state->setTextureAttributeAndModes(COLOR, m_fboTextures[COLOR], osg::StateAttribute::ON);
	//state->setTextureAttributeAndModes(NORMALDEPTH, m_fboTextures[NORMALDEPTH], osg::StateAttribute::ON);
	state->setTextureAttributeAndModes(ID, m_fboTextures[ID], osg::StateAttribute::ON);
	state->setTextureAttributeAndModes(PONG, m_fboTextures[PONG], osg::StateAttribute::ON);
	state->setTextureAttributeAndModes(OLDCOLOR, m_fboTextures[OLDCOLOR], osg::StateAttribute::ON);

	return postRenderCamera;
}

bool PostProcessing::handleGuiEvents(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*)
{
	if (ea.getEventType() == osgGA::GUIEventAdapter::RESIZE)
	{
		// re setup textures to new size
		setupTextures(ea.getWindowWidth(), ea.getWindowHeight());
		return true;
	}
	else return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////// Oculus Specific Code ///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


osg::ref_ptr<osg::Camera> PostProcessing::oculusPass(bool left)
{
	// /////////////
	osg::ref_ptr<osg::Camera> cam = new osg::Camera();
	TEXTURE_CONTENT SIDE = left ? LEFT : RIGHT;
	
	cam->attach((osg::Camera::BufferComponent)(osg::Camera::COLOR_BUFFER0), m_fboTextures[SIDE]);
	cam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Configure fboCamera to draw fullscreen textured quad
	// black clear color
	cam->setClearColor(osg::Vec4(0.0, 0.0, 0.5, 1.0));
	cam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

	cam->setReferenceFrame(osg::Camera::RELATIVE_RF);
	cam->setRenderOrder( osg::Camera::PRE_RENDER, 0);

	cam->addChild(m_sceneNode);

	

	// calculation


	// for stereo rendering.
	OVR::Util::Render::StereoConfig stereo;
	stereo.SetFullViewport(OVR::Util::Render::Viewport(0, 0, m_width, m_height));
	stereo.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
	stereo.SetHMDInfo(*m_hmd);
	stereo.SetDistortionFitPointVP(-1.0f, 0.0f);
	float renderScale = stereo.GetDistortionScale();
	


	OVR::Util::Render::StereoEye eye = left ? OVR::Util::Render::StereoEye_Left : OVR::Util::Render::StereoEye_Right;

	
	OVR::Util::Render::StereoEyeParams eyeParams = stereo.GetEyeRenderParams(eye);
	OVR::Util::Render::Viewport eyeVP = eyeParams.VP;
	OVR::Matrix4f eyeProjection = eyeParams.Projection;
	OVR::Matrix4f eyeViewAdjust = eyeParams.ViewAdjust;
		
	osg::Matrixf eyeProjectionOSG(*eyeProjection.M);
	osg::Matrixf eyeViewAdjustOSG(*eyeViewAdjust.M);

	//osg::Matrixf v = m_camera->getViewMatrix();
	//osg::Matrixf p = m_camera->getProjectionMatrix();

	//cam->setProjectionMatrix(leftProjectionOSG);


	osg::ref_ptr<osg::StateSet>	state = cam->getOrCreateStateSet();

	state->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::OCULUS_EYE], osg::StateAttribute::ON);

	state->addUniform(new osg::Uniform("occViewAdjust", eyeViewAdjustOSG));
	state->addUniform(new osg::Uniform("occProjection", eyeProjectionOSG));

	//cam->setViewport(leftVP.x, leftVP.y, leftVP.w, leftVP.h);

	
	return cam;
}


osg::ref_ptr<osg::Camera> PostProcessing::mergeEyes()
{
	osg::ref_ptr<osg::Camera> postRenderCamera(new osg::Camera());

	// input textures
	//postRenderCamera->attach((osg::Camera::BufferComponent) (osg::Camera::COLOR_BUFFER0 + PING), m_fboTextures[PING]);
	//postRenderCamera->attach((osg::Camera::BufferComponent) (osg::Camera::COLOR_BUFFER0 + COLOR), m_fboTextures[COLOR]);

	// configure postRenderCamera to draw fullscreen textured quad
	postRenderCamera->setClearColor(osg::Vec4(0.0, 0.5, 0.0, 1)); // should never see this.
	postRenderCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	postRenderCamera->setRenderOrder(osg::Camera::POST_RENDER, 3);

	// geometry
	/*geode->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(-1, -1, 0), osg::Vec3(2, 0, 0), osg::Vec3(0, 2, 0)));*/
	
	osg::Geode* geodeLeft(new osg::Geode());
	geodeLeft->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(-1, -1, 0), osg::Vec3(1, 0, 0), osg::Vec3(0, 2, 0)));
	geodeLeft->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osg::Geode* geodeRight(new osg::Geode());
	geodeRight->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(0, -1, 0), osg::Vec3(1, 0, 0), osg::Vec3(0, 2, 0)));
	geodeRight->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	
	postRenderCamera->addChild(geodeLeft);
	postRenderCamera->addChild(geodeRight);
	

	// attach shader program
	osg::ref_ptr<osg::StateSet>	state = postRenderCamera->getOrCreateStateSet();
	state->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	state->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::OCULUS_MERGE], osg::StateAttribute::ON);


	osg::ref_ptr<osg::StateSet>	stateLeft = geodeLeft->getOrCreateStateSet();
	stateLeft->addUniform(new osg::Uniform("side", LEFT));
	stateLeft->setTextureAttributeAndModes(LEFT, m_fboTextures[LEFT], osg::StateAttribute::ON);
	
	osg::ref_ptr<osg::StateSet>	stateRight = geodeRight->getOrCreateStateSet();
	stateRight->addUniform(new osg::Uniform("side", RIGHT));
	stateRight->setTextureAttributeAndModes(RIGHT, m_fboTextures[RIGHT], osg::StateAttribute::ON);





	// remove

	OVR::Util::Render::StereoConfig stereo;
	stereo.SetFullViewport(OVR::Util::Render::Viewport(0, 0, m_width, m_height));
	stereo.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
	stereo.SetHMDInfo(*m_hmd);
	stereo.SetDistortionFitPointVP(-1.0f, 0.0f);
	float renderScale = stereo.GetDistortionScale();


	bool left = true;
	OVR::Util::Render::StereoEye eye = left ? OVR::Util::Render::StereoEye_Left : OVR::Util::Render::StereoEye_Right;


	OVR::Util::Render::StereoEyeParams eyeParams = stereo.GetEyeRenderParams(eye);
	OVR::Util::Render::Viewport eyeVP = eyeParams.VP;
	OVR::Matrix4f eyeProjection = eyeParams.Projection;
	OVR::Matrix4f eyeViewAdjust = eyeParams.ViewAdjust;

	osg::Matrixf eyeProjectionOSG(*eyeProjection.M);
	osg::Matrixf eyeViewAdjustOSG(*eyeViewAdjust.M);

	//osg::Matrixf v = m_camera->getViewMatrix();
	//osg::Matrixf p = m_camera->getProjectionMatrix();

	//cam->setProjectionMatrix(leftProjectionOSG);



	state->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::OCULUS_EYE], osg::StateAttribute::ON);

	state->addUniform(new osg::Uniform("occViewAdjust", eyeViewAdjustOSG));
	state->addUniform(new osg::Uniform("occProjection", eyeProjectionOSG));


	// remove end









	return postRenderCamera;
}





