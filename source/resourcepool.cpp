#include "ResourcePool.h"

#include <osgDB/ReadFile>


using namespace troen;

ResourcePool::ResourcePool() {
	std::string folder = "data/models/cycle/";
	const int textureCount = 21;
	std::string fileNames[textureCount] = {
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


	for (int i = 0; i < textureCount; ++i)
	{
		const std::string filePath = folder + fileNames[i];
		m_images[i] = osgDB::readImageFile(filePath);
	}

}

ResourcePool::~ResourcePool()
{

}

osg::Image* ResourcePool::getImage(TextureResource texture)
{
	return m_images[texture];
}