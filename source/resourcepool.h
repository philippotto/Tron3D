#pragma once
// OSG

// troen
#include "forwarddeclarations.h"

namespace troen
{
	class ResourcePool
	{


	public:
		ResourcePool();
		~ResourcePool();


		enum TextureResource
		{
			MG_MovieCycle_Body_SPEC,
			MG_MovieCycle_BodyHeadLight_EMSS,
			MG_MovieCycle_Body_NORM,
			MG_Player_Body_SPEC,
			MG_Player_Body_EMSS,
			MG_Player_Body_NORM,
			MG_MovieCycle_Tire_DIFF,
			MG_MovieCycle_Tire_EMSS,
			MG_MovieCycle_Tire_NORM,
			MG_Player_Helmet_SPEC,
			MG_Player_Helmet_EMSS,
			MG_Player_Helmet_NORM,
			MG_Player_Disc_SPEC,
			MG_Player_Disc_EMSS,
			MG_Player_Disc_NORM,
			Glass,
			MG_MovieCycle_Engine_EMSS,
			MG_MovieCycle_Engine_NORM,
			MG_Player_Baton_SPEC,
			MG_Player_Baton_EMSS,
			MG_Player_Baton_NORM,
			None
		};

		osg::Image* ResourcePool::getImage(TextureResource texture);

		
	private:
		osg::Image *m_images[22];
		
	};
}