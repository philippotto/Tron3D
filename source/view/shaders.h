#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Geode>
// troen
#include "../forwarddeclarations.h"

/* \file This file is responsible for (re)loading the shader files into osg::Programs.*/

namespace troen
{
	namespace shaders
	{		
		#ifndef _SHADERS_H
		#define _SHADERS_H 
				extern std::vector<osg::ref_ptr<osg::Program> > m_allShaderPrograms;
		#endif

		bool loadShaderSource(osg::Shader* obj, const std::string& fileName);
		void reloadShader(osg::ref_ptr<osg::Program> & program,
			std::string fragmentFileName,
			std::string vertexFileName,
			std::string secondVertexFileName);

		void reloadShaders();


		enum Shadernames  {
							DEFAULT,
							BIKE,
							GRID,
							GRID_NOREFLECTION,
							FENCE,
							GBUFFER,
							HBLUR,
							VBLUR,
							SELECT_GLOW_OBJECTS,
							POST_PROCESSING,
							SKYDOME,
							PLAYERMARKER,
							SHADER_NAME_COUNT //always last, isnt a shader
						};
	}

}