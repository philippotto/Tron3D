#pragma once
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

		enum ModelResource
		{
			MG_MovieCycle_Body_MI,
			MG_MovieCycle_PlayerBody_MI,
			MG_MovieCycle_Tire_MI,
			MG_MovieCycle_PlayerHelmet_MI,
			MG_MovieCycle_PlayerDisc_MI,
			MG_MovieCycle_Glass_MI,
			MG_MovieCycle_Engine_MI,
			MG_MovieCycle_PlayerBaton_MI,
		};

		void readData();

		osg::Image* getImage(const TextureResource texture);
		osg::Node* getNode(const ModelResource model);

	private:
		static const int m_textureCount = 21;
		osg::Image *m_images[m_textureCount];

		static const int m_objectCount = 8;
		osg::Node *m_objects[m_objectCount];

		bool m_dataAlreadyRead = false;
	};
}