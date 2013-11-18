#pragma once
// OSG
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <osg/Shader>
#include <osg/Program>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"


namespace troen
{

	class BikeView : public AbstractView //, public osg::Referenced
	{
	public:
		BikeView();

		osg::ref_ptr<osg::MatrixTransform> get_rootNode();
		void setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath);
	
	//protected:
	//	osg::ref_ptr<osg::Program> m_program;
	//	osg::ref_ptr<osg::Shader> m_vShader;
	//	osg::ref_ptr<osg::Shader> m_fShader;

	private:
		//osg::ref_ptr<osg::Group> rootNode;
		osg::ref_ptr<osg::MatrixTransform> rootNode;


	};
}