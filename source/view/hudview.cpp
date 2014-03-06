#include "hudview.h"
// osg
#include <osgUtil/Optimizer>
#include <osgText/Text>
#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/PolygonOffset>
#include <osg/MatrixTransform>
#include <osg/RenderInfo>
#include <osgText/Text>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/View>
#include <osgGA/NodeTrackerManipulator>
#include <osg/Quat>
// troen
#include "../player.h"
#include "../constants.h"

using namespace troen;

HUDView::HUDView(const int i, const std::vector<std::shared_ptr<Player>>& players) :
AbstractView(),
m_trackNode(nullptr),
m_healthText(new osgText::Text()),
m_speedText(new osgText::Text()),
m_pointsText(new osgText::Text()),
m_countdownText(new osgText::Text()),
m_timeText(new osgText::Text()),
m_playerColor(osg::Vec4(players[i]->color(),1))
{
	m_node->addChild(createHUD(players));
	m_node->addChild(createRadar(i));
}

osg::ref_ptr<osg::Camera> HUDView::createHUD(const std::vector<std::shared_ptr<Player>>& players)
{
	// create a camera to set up the projection & 
	// model view matrices and the subgraph to draw in the HUD
	m_camera = new osg::Camera;
	m_camera->setViewport(
		new osg::Viewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
	m_camera->setProjectionMatrix(
		osg::Matrix::ortho2D(0, HUD_PROJECTION_SIZE, 0, HUD_PROJECTION_SIZE));
	m_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	m_camera->setViewMatrix(osg::Matrix::identity());
	m_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	m_camera->setAllowEventFocus(false);
	// draw subgraph after main camera view.
	m_camera->setRenderOrder(osg::Camera::POST_RENDER);

	m_savedGeode = new osg::Geode();
	m_camera->addChild(m_savedGeode);

	m_font = osgText::readFontFile("data/fonts/tr2n.ttf");

	////////////////////////////////////////////////////////////////////////////////
	//
	// Main HUD elements
	//
	////////////////////////////////////////////////////////////////////////////////

	// turn lighting off for the text and disable
	// depth test to ensure it's always on top.
	osg::StateSet* stateset = m_savedGeode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	int offset = HUD_PROJECTION_SIZE / 20;

	initializeText(
		m_speedText,
		osg::Vec3(HUD_PROJECTION_SIZE - offset, offset, 0),
		m_playerColor,
		osgText::Text::AlignmentType::RIGHT_BOTTOM,
		DEFAULT_WINDOW_HEIGHT / 15);
	setSpeedText(0);
	m_savedGeode->addDrawable(m_speedText);

	initializeText(
		m_healthText,
		osg::Vec3(offset, offset, 0.f),
		m_playerColor,
		osgText::Text::AlignmentType::LEFT_BOTTOM,
		DEFAULT_WINDOW_HEIGHT / 15);
	setHealthText(100);
	m_savedGeode->addDrawable(m_healthText);

	initializeText(
		m_pointsText,
		osg::Vec3(offset, HUD_PROJECTION_SIZE - offset, 0.f),
		m_playerColor,
		osgText::Text::AlignmentType::LEFT_TOP,
		DEFAULT_WINDOW_HEIGHT / 15);
	setPointsText(0);
	m_savedGeode->addDrawable(m_pointsText);

	initializeText(
		m_countdownText,
		osg::Vec3(HUD_PROJECTION_SIZE / 2, HUD_PROJECTION_SIZE / 2, 0.f),
		m_playerColor,
		osgText::Text::AlignmentType::CENTER_CENTER,
		DEFAULT_WINDOW_HEIGHT / 3);
	setCountdownText(-1);
	m_savedGeode->addDrawable(m_countdownText);

	initializeText(
		m_timeText,
		osg::Vec3(HUD_PROJECTION_SIZE - offset, HUD_PROJECTION_SIZE - offset, 0.f),
		m_playerColor,
		osgText::Text::AlignmentType::RIGHT_TOP,
		DEFAULT_WINDOW_HEIGHT / 8);
	setTimeText(-1, -1);
	m_savedGeode->addDrawable(m_timeText);


	for (int i = 0; i < players.size(); i++)
	{
		m_killCountTexts[i] = new osgText::Text();
		initializeText(
			m_killCountTexts[i],
			osg::Vec3(HUD_PROJECTION_SIZE - offset, HUD_PROJECTION_SIZE - offset * (3 + i), 0.f),
			osg::Vec4(players[i]->color(), 1),
			osgText::Text::AlignmentType::RIGHT_TOP,
			DEFAULT_WINDOW_HEIGHT / 20);
		setKillCountText(i, players[i]->name(), 0);
		m_savedGeode->addDrawable(m_killCountTexts[i]);
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	// Ingame Message Texts
	//
	////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 4; i++)
	{
		m_ingameMessageTexts[i] = new osgText::Text();
		initializeText(
			m_ingameMessageTexts[i],
			osg::Vec3(HUD_PROJECTION_SIZE / 2.f, HUD_PROJECTION_SIZE - offset * (5 + i*2.f), 0.f),
			osg::Vec4(1, 1, 1, 1),
			osgText::Text::AlignmentType::CENTER_TOP,
			DEFAULT_WINDOW_HEIGHT / 20);
		m_ingameMessageTexts[i]->setText("");
		m_savedGeode->addDrawable(m_ingameMessageTexts[i]);
	}

	return m_camera;
}

void HUDView::initializeText(
	osg::ref_ptr<osgText::Text> text,
	const osg::Vec3& position,
	const osg::Vec4& color,
	const osgText::Text::AlignmentType alignment,
	const int size)
{
	text->setFont(m_font);
	text->setPosition(position);
	text->setColor(color);
	text->setAlignment(alignment);
	text->setCharacterSizeMode(osgText::TextBase::CharacterSizeMode::SCREEN_COORDS);
	text->setCharacterSize(size);
	text->setFontResolution(size, size);
}


void HUDView::resize(const int width,const int height)
{
	osg::ref_ptr<osg::Viewport> hudViewport = new osg::Viewport(0, 0, width, height);
	m_camera->setViewport(hudViewport);
	resizeHudComponents(width, height);

	int smallerBound = height < width ? height : width;
	int offsetX = smallerBound / 20;
	int offsetY = height / 10;
	int size = smallerBound / 3;
	osg::ref_ptr<osg::Viewport> radarViewport = new osg::Viewport(offsetX, offsetY, size, size);
	m_radarCamera->setViewport(radarViewport);
}

void HUDView::resizeHudComponents(const int width, const int height)
{
	m_speedText->setCharacterSize(height / 15);
	m_speedText->setFontResolution(height / 15, height / 15);

	m_healthText->setCharacterSize(height / 15);
	m_healthText->setFontResolution(height / 15, height / 15);

	m_pointsText->setCharacterSize(height / 15);
	m_pointsText->setFontResolution(height / 15, height / 15);

	m_countdownText->setCharacterSize(height / 3);
	m_countdownText->setFontResolution(height / 3, height / 3);

	m_timeText->setCharacterSize(height / 8);
	m_timeText->setFontResolution(height / 8, height / 8);
	
	const int textNum = sizeof(m_killCountTexts) / sizeof(m_killCountTexts[0]);
	for (size_t i = 0; i < textNum; i++)
	{
		if (m_killCountTexts[i].valid())
		{
			m_killCountTexts[i]->setCharacterSize(height / 15);
			m_killCountTexts[i]->setFontResolution(height / 15, height / 15);
		}
	}

	for (size_t i = 0; i < 4; i++)
	{
		m_ingameMessageTexts[i]->setCharacterSize(height / 20);
		m_ingameMessageTexts[i]->setFontResolution(height / 20, height / 20);
	}
}

osg::ref_ptr<osg::Camera> HUDView::createRadar(const int i)
{
	m_radarCamera = new osg::Camera;
	m_radarCamera->setClearColor(osg::Vec4(0.0f, 1.f, 0.0f, .5f));
	m_radarCamera->setRenderOrder(osg::Camera::POST_RENDER);
	m_radarCamera->setAllowEventFocus(false);
	m_radarCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	m_radarCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	int offset = DEFAULT_WINDOW_HEIGHT / 20;
	int size = DEFAULT_WINDOW_HEIGHT / 3;
	m_radarCamera->setViewport(offset, offset * 2, size, size);

	m_radarCamera->setViewMatrix(osg::Matrixd::lookAt(osg::Vec3(0.0f, 0.0f, 500.0f), osg::Vec3(0.f, 0.f, 0.f), osg::Y_AXIS));
	m_radarCamera->setProjectionMatrix(osg::Matrixd::ortho(-3000, 3000, -3000, 3000, 1.f, 600));
	m_radarCamera->setCullMask(CAMERA_MASK_RADAR | CAMERA_MASK_PLAYER[i]);

	return m_radarCamera;
}

void HUDView::attachSceneToRadarCamera(osg::Group* scene)
{
	osg::ref_ptr<osg::Group> hudGroup = new osg::Group;

	osg::StateSet* stateset = hudGroup->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	hudGroup->addChild(scene);
	m_radarCamera->addChild(hudGroup);
}

// helper class to retrieve the position of a node in world coordinates
// could potentially be in it's own header file
class getWorldCoordOfNodeVisitor : public osg::NodeVisitor
{
public:
    getWorldCoordOfNodeVisitor():
    osg::NodeVisitor(NodeVisitor::TRAVERSE_PARENTS), done(false)
    {
        wcMatrix= new osg::Matrixd();
    }
    virtual void apply(osg::Node &node)
    {
        if (!done)
        {
            if ( 0 == node.getNumParents() ) // no parents
            {
                wcMatrix->set( osg::computeLocalToWorld(this->getNodePath()));
                done = true;
            }
            traverse(node);
        }
    }
    osg::Matrixd* worldCoordinatesMatrix()
    {
        return wcMatrix;
    }
private:
    bool done;
    osg::Matrix* wcMatrix;
};

void HUDView::updateRadarCamera()
{
        osg::Matrixd* worldCoordinateMatrix = nullptr;
		getWorldCoordOfNodeVisitor ncv;
        
		// fail silently, in case no node is attached
		if (!m_trackNode)
		{
			std::cout << "[HUDView::updateRadarCamera] updating HUDView Radar without node attached" << std::endl;
			return;
		}

        dynamic_cast<osg::Group*>(m_trackNode.get())->accept(ncv);
        worldCoordinateMatrix = ncv.worldCoordinatesMatrix();
            
        osg::Vec3d position = worldCoordinateMatrix->getTrans();
        osg::Quat rotationQuat = worldCoordinateMatrix->getRotate();
		float z = rotationQuat.z();
		float w = rotationQuat.w();
        float mag = sqrt(w*w + z*z);
        rotationQuat.set(0, 0, z/mag, w/mag);
            
        osg::Vec3 up = rotationQuat * osg::Y_AXIS;
            
        osg::Matrixd viewMatrix = osg::Matrixd::lookAt(osg::Vec3(position.x(), position.y(), 500.0f), osg::Vec3(position.x(), position.y(), 0.f), up);
        m_radarCamera->setViewMatrix(viewMatrix);
}

void HUDView::setTrackNode(osg::Node* trackNode)
{
    m_trackNode = trackNode;
}


void HUDView::setSpeedText(float speed)
{
	std::string speedString = std::to_string((int) speed);
	m_speedText->setText(speedString + " km/h");
}

void HUDView::setHealthText(float health)
{
	std::string healthString = std::to_string((int)health);
	m_healthText->setText("Health: " + healthString);
}

void HUDView::setPointsText(float points)
{
	std::string pointsString = std::to_string((int)points);
	m_pointsText->setText("Points: " + pointsString);
}

void HUDView::setCountdownText(const int countdown)
{
	if (countdown == -1)
	{
		m_countdownText->setText("");
	}
	else
	{
		std::string countdownString = std::to_string(countdown);
		m_countdownText->setText(countdownString);
	}
}

void HUDView::setCountdownText(const std::string text)
{
	m_countdownText->setText(text);
}


void HUDView::setTimeText(const double gameTime, const int timeLimit)
{

	if (gameTime <= 0)
	{
		m_timeText->setText("0:00");
	}
	else if ((gameTime/60/1000) >= timeLimit)
	{
		//m_timeText->setText("0:00");
		int minutes = abs(timeLimit - floor((gameTime) / 1000 / 60));
		int seconds = mod(floor((gameTime) / 1000), 60);
		//int milliSeconds = mod(time/10, 100);
		std::string timeString = "-" + std::to_string(minutes) + ":" + std::to_string(seconds);// +":" + std::to_string(milliSeconds);
		m_timeText->setText(timeString);
	}
	else
	{
		int minutes = timeLimit - ceil((gameTime) / 1000 / 60);
		int seconds = 59 - mod(floor((gameTime) / 1000), 60);
		//int milliSeconds = mod(time/10, 100);
		std::string timeString = std::to_string(minutes) + ":" + std::to_string(seconds);// +":" + std::to_string(milliSeconds);
		m_timeText->setText(timeString);
	}
}

void HUDView::setKillCountText(const int i, const std::string& playerName , const int killCount)
{
	m_killCountTexts[i]->setText(playerName + ": " + std::to_string(killCount));
}

void HUDView::updateIngameMessageTexts(std::deque<std::shared_ptr<IngameMessage>>& messages)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (i < messages.size())
		{
			m_ingameMessageTexts[i]->setText(messages[i]->text);
			m_ingameMessageTexts[i]->setColor(messages[i]->color);
		}
		else
		{
			m_ingameMessageTexts[i]->setText("");
		}
	}
}
