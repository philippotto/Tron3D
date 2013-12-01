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


using namespace troen;

HUDView::HUDView()
{
	m_node = new osg::Group();
	
	m_node->addChild(createHUD());
}




osg::Camera* HUDView::createHUD()
{
	// create a camera to set up the projection and model view matrices, and the subgraph to draw in the HUD
	osg::Camera* camera = new osg::Camera;

	// set the projection matrix
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1280, 0, 1024));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	// draw subgraph after main camera view.
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setAllowEventFocus(false);

	
	{
		osg::Geode* geode = new osg::Geode();

		osgText::Font *timesFont = osgText::readFontFile("../data/fonts/Tr2n.ttf");

		// turn lighting off for the text and disable depth test to ensure it's always ontop.
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		osg::Vec3 position(0, 1000, 0);
		osg::Vec3 delta(0.0f, -100.0f, 0.0f);

		{
			osgText::Text* text = new  osgText::Text;
			geode->addDrawable(text);

			text->setFont(timesFont);
			text->setPosition(position);
			text->setText("Player 1 - This is your Head Up Display");

			position += delta;
		}


		/*{
			osgText::Text* text = new  osgText::Text;
			geode->addDrawable(text);

			text->setFont(timesFont);
			text->setPosition(position);
			text->setText("All you need to do is create your text in a subgraph.");

			position += delta;
		}
*/


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

		camera->addChild(geode);
	}

	return camera;
}
