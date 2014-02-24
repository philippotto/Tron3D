#include "ResourcePool.h"

#include <osgDB/ReadFile>


using namespace troen;

ResourcePool::ResourcePool() {}

ResourcePool::~ResourcePool() {}

void ResourcePool::readData()
{
	m_dataAlreadyRead = true;

	std::string folder = "data/models/cycle/";

	std::string textureFileNames[m_textureCount] = {
		"MG_MovieCycle_Body_SPEC.tga",
		"MG_MovieCycle_BodyHeadLight_EMSS.tga",
		"MG_MovieCycle_Body_NORM.tga",
		"MG_Player_Body_SPEC.tga",
		"MG_Player_Body_EMSS.tga",
		"MG_Player_Body_NORM.tga",
		"MG_MovieCycle_Tire_DIFF.tga",
		"MG_MovieCycle_Tire_EMSS.tga",
		"MG_MovieCycle_Tire_NORM.tga",
		"MG_Player_Helmet_SPEC.tga",
		"MG_Player_Helmet_EMSS.tga",
		"MG_Player_Helmet_NORM.tga",
		"MG_Player_Disc_SPEC.tga",
		"MG_Player_Disc_EMSS.tga",
		"MG_Player_Disc_NORM.tga",
		"Glass.tga",
		"MG_MovieCycle_Engine_EMSS.tga",
		"MG_MovieCycle_Engine_NORM.tga",
		"MG_Player_Baton_SPEC.tga",
		"MG_Player_Baton_EMSS.tga",
		"MG_Player_Baton_NORM.tga"
	};

	for (int i = 0; i < m_textureCount; ++i)
	{
		const std::string filePath = folder + textureFileNames[i];
		//m_images[i] = osgDB::readImageFile(filePath);
		
		osg::ref_ptr<osg::Image> img = osgDB::readImageFile(filePath);
		m_images[i] = img.release();
		
	}


	std::string objFileNames[m_objectCount] = {
		"MG_MovieCycle_Body_MI.ive",
		"MG_MovieCycle_PlayerBody_MI.ive",
		"MG_MovieCycle_Tire_MI.ive",
		"MG_MovieCycle_PlayerHelmet_MI.ive",
		"MG_MovieCycle_PlayerDisc_MI.ive",
		"MG_MovieCycle_Glass_MI.ive",
		"MG_MovieCycle_Engine_MI.ive",
		"MG_MovieCycle_PlayerBaton_MI.ive"
	};

	for (int i = 0; i < m_objectCount; ++i)
	{
		const std::string filePath = folder + objFileNames[i];
		std::cout << "[TroenGame::bikeView] Loading Model \"" << filePath << "\"" << std::endl;
		m_objects[i] = osgDB::readNodeFile(filePath);
	}

}

osg::Image* ResourcePool::getImage(TextureResource texture)
{
	if (!m_dataAlreadyRead)
		readData();

	if (!m_images[texture]->valid() || !m_images[texture] || m_images[texture]->getImageSizeInBytes() <= 0) {
		std::cout << "rereading data for texture: " << texture << std::endl;
		readData();	
	}

	return m_images[texture];
}


osg::Node* ResourcePool::getNode(ModelResource model)
{
	if (!m_dataAlreadyRead)
		readData();

	const osg::CopyOp copyOp(osg::CopyOp::SHALLOW_COPY);
	return dynamic_cast<osg::Node*>(m_objects[model]->clone(copyOp));
}