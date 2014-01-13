#include "hudview.h"

// osg
#include <osgUtil/Optimizer>
#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/PolygonOffset>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/RenderInfo>
#include <osgText/Text>
// troen
#include "../constants.h"


using namespace troen;

HUDView::HUDView()
{
	m_speedText = new osgText::Text();	
	
	m_node = new osg::Group();
	
	m_node->addChild(createHUD());
	m_node->addChild(createRadar());
}

//HUDView::~HUDView()
// {
	// TODO: destructor cannot be accessed ?
	// delete m_speedText;
// }


osg::Camera* HUDView::createHUD()
{
	// create a camera to set up the projection and model view matrices, and the subgraph to draw in the HUD
	m_camera = new osg::Camera;

	// set the projection matrix
	m_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, DEFAULT_WINDOW_HEIGHT, 0, DEFAULT_WINDOW_WIDTH));
	m_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	m_camera->setViewMatrix(osg::Matrix::identity());
	m_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	// draw subgraph after main camera view.
	m_camera->setRenderOrder(osg::Camera::POST_RENDER);
	m_camera->setAllowEventFocus(false);

	{
		osg::Geode* geode = new osg::Geode();
		m_savedGeode = geode;

		osgText::Font *timesFont = osgText::readFontFile("../data/fonts/Tr2n.ttf");

		// turn lighting off for the text and disable depth test to ensure it's always ontop.
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		osg::Vec3 position(500.f, 0, 0);
		osg::Vec3 delta(0.0f, -100.0f, 0.0f);

		{
			geode->addDrawable(m_speedText);

			m_speedText->setFont(timesFont);
			m_speedText->setPosition(position);
			m_speedText->setText("Speed: 20 km/h");
			// m_speedText->setAlignment(osgText::Text::AlignmentType::RIGHT_BOTTOM);

			position += delta;
		}
		/*{
			osgText::Text* text = new  osgText::Text;
			geode->addDrawable(text);

			text->setFont(timesFont);
			text->setPosition(position);
			text->setText("All you need to do is create your text in a subgraph.");

			position += delta;
		}*/

		{
			osg::BoundingBox bb;
			for (unsigned int i = 0; i<geode->getNumDrawables(); ++i)
			{
				bb.expandBy(geode->getDrawable(i)->getBound());
			}

			osg::Geometry* geom = new osg::Geometry;

			osg::Vec3Array* vertices = new osg::Vec3Array;
			float depth = bb.zMin() - 0.1;
			vertices->push_back(osg::Vec3(bb.xMin(), bb.yMax(), depth));
			vertices->push_back(osg::Vec3(bb.xMin(), bb.yMin(), depth));
			vertices->push_back(osg::Vec3(bb.xMax(), bb.yMin(), depth));
			vertices->push_back(osg::Vec3(bb.xMax(), bb.yMax(), depth));
			geom->setVertexArray(vertices);
		
			osg::Vec3Array* normals = new osg::Vec3Array;
			normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
			geom->setNormalArray(normals);
			geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

			osg::Vec4Array* colors = new osg::Vec4Array;
			colors->push_back(osg::Vec4(1.0f, 1.0, 0.8f, 0.2f));
			geom->setColorArray(colors);
			geom->setColorBinding(osg::Geometry::BIND_OVERALL);

			geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

			osg::StateSet* stateset = geom->getOrCreateStateSet();
			stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
			//stateset->setAttribute(new osg::PolygonOffset(1.0f,1.0f),osg::StateAttribute::ON);
			stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

			geode->addDrawable(geom);
		}

		m_camera->addChild(geode);
	}

	return m_camera;
}


void HUDView::resize(int width, int height)
{
	m_camera->setViewport(new osg::Viewport(0, 0, width, height));
	m_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, height, 0, width));

}

osg::Camera* HUDView::createRadar()
{
	m_radarCamera = new osg::Camera;
	m_radarCamera->setClearColor(osg::Vec4(0.0f, 1.f, 0.0f, .5f));
	m_radarCamera->setRenderOrder(osg::Camera::POST_RENDER);
	m_radarCamera->setAllowEventFocus(false);
	m_radarCamera->setClearMask(/**/GL_COLOR_BUFFER_BIT|/**/GL_DEPTH_BUFFER_BIT);
	m_radarCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	m_radarCamera->setViewport(0.0, 0.0, 400.0, 400.0);

	m_radarCamera->setViewMatrix(osg::Matrixd::lookAt(osg::Vec3(0.0f, 0.0f, 200.0f), osg::Vec3(0.f, 0.f, 0.f), osg::Y_AXIS));
	m_radarCamera->setProjectionMatrix(osg::Matrixd::ortho(-3000, 3000, -3000, 3000.,1.f,600));
	m_radarCamera->setCullMask(CAMERA_MASK_RADAR | CAMERA_MASK_MAIN);
	
	return m_radarCamera;
}

void HUDView::attachSceneToRadarCamera(osg::Group* scene)
{

	m_radarCamera->addChild(scene);
}

void HUDView::setSpeedText(float speed)
{
	std::string speedString = std::to_string((int) speed);
	m_speedText->setText("Speed: " + speedString + " km/h");
}